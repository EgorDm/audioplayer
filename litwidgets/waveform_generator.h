//
// Created by egordm on 3-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <utils/debug.h>

using namespace litsignal::algorithm;

namespace litwidgets {
    struct WaveformContext : public AlgorithmContext<arma::frowvec> {
        WaveformContext() : AlgorithmContext(arma::frowvec2()) {}
    };

    class WaveformGeneratorAlgorithm : public AlgorithmInterface<arma::fvec, WaveformContext> {
    public:
        WaveformGeneratorAlgorithm() = default;

        void processFrame(WaveformContext &context, arma::fvec &frame, int i) override {
            context.getOutput()(0) = abs(frame).max(); // Max value
            auto test =  abs(frame).max();
            context.getOutput()(1) = sqrtf(arma::dot(frame, frame) / frame.size()); // RMS
        }

        WaveformContext createContext(arma::fvec &frame) override {
            return WaveformContext();
        }
    };

    using WaveformPipeline = AlgorithmPipeline<fvec, frowvec, fmat, WaveformContext, WaveformGeneratorAlgorithm>;

    class WaveformGenerator {
    private:
        AlgorithmSimpleRunner<WaveformPipeline> runner;
        WaveformPipeline *pipeline;

        int width;
        bool invalidated = true;

    public:
        WaveformGenerator(FrameFactoryInterface<Col<float>> *frameFactory, int width)
                : width(width), invalidated(true), pipeline(
                new WaveformPipeline(frameFactory, new OutputBuilderRowMat<float>(2), WaveformGeneratorAlgorithm())) {
            if(frameFactory) setInput(frameFactory);
        }

        virtual ~WaveformGenerator() {
            delete pipeline;
        }

        void recalculate() {
            if (!invalidated || !pipeline->getFrameFactory()) return;
            pipeline->getOutputBuilder()->reset();
            runner.run(pipeline);
            getOutput()(span::all, 0) /= getOutput()(span::all, 0).max();
            invalidated = false;
        }

        fmat &getOutput() {
            return pipeline->getOutputBuilder()->getOutput();
        }

        void setInput(FrameFactoryInterface<Col<float>> *frameFactory) {
            pipeline->setFrameFactory(frameFactory);
            if (dynamic_cast<FrameHopInterface *>(pipeline->getFrameFactory()) == nullptr) {
                litcore::debug::log_error("WaveformGeneratorAlgorithm",
                                 "Cannot use FrameFactory that doesnt support FrameHopInterface");
                return;
            }
            updateSize();
        }

        void setWidth(int width) {
            if (WaveformGenerator::width == width) return;
            WaveformGenerator::width = width;
            updateSize();
        }

        bool isInvalidated() const {
            return invalidated;
        }

    private:
        void updateSize() {
            if(!pipeline->getFrameFactory()) return;
            int frame_size = (int) std::ceil(pipeline->getFrameFactory()->getInputSize() / (float) width);
            auto fhi = dynamic_cast<FrameHopInterface *>(pipeline->getFrameFactory());
            fhi->setFrameSize(frame_size);
            fhi->setHopSize(frame_size);
            pipeline->getOutputBuilder()->reset();
            pipeline->getOutputBuilder()->resize(pipeline->getFrameFactory()->getFrameCount());
            invalidated = true;
        }
    };
}

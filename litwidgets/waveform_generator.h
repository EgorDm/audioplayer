//
// Created by egordm on 3-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>

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
            context.getOutput()(1) = sqrtf(arma::dot(frame, frame) / frame.size()); // RMS
        }

        WaveformContext createContext(arma::fvec &frame) override {
            return WaveformContext();
        }
    };

    using WaveformPipeline = AlgorithmPipeline<fvec, fvec, frowvec, fmat, WaveformContext, WaveformGeneratorAlgorithm>;

    class WaveformGenerator {
    private:
        AlgorithmSimpleRunner<WaveformPipeline> runner;
        FrameFactoryVec<float> *frameFactory = nullptr;
        WaveformPipeline *pipeline;

        fvec &input;
        int width;
        bool invalidated = true;

    public:
        WaveformGenerator(fvec &input, int width) : input(input), width(width), invalidated(true) {
            frameFactory = new FrameFactoryVec<float>(input, 1, 1);
            pipeline = new WaveformPipeline(frameFactory, new OutputBuilderRowMat<float>(2), WaveformGeneratorAlgorithm());
        }

        virtual ~WaveformGenerator() {
            delete pipeline;
        }

        void recalculate() {
            if(!invalidated) return;
            pipeline->getOutputBuilder()->reset();
            runner.run(pipeline);
            getOutput()(span::all, 0) /= getOutput()(span::all, 0).max();
            invalidated = false;
        }

        fmat &getOutput() {
            return pipeline->getOutputBuilder()->getOutput();
        }

        void setInput(fvec &input) {
            WaveformGenerator::input = input;
            updateSize();
        }

        void setWidth(int width) {
            if(WaveformGenerator::width == width) return;
            WaveformGenerator::width = width;
            updateSize();
        }

        bool isInvalidated() const {
            return invalidated;
        }

    private:
        void updateSize() {
            int frame_size = (int) std::ceil(input.size() / (float) width);
            frameFactory->setFrameSize(frame_size);
            frameFactory->setHopSize(frame_size);
            pipeline->getOutputBuilder()->reset();
            pipeline->getOutputBuilder()->resize(frameFactory->getFrameCount());
            invalidated = true;
        }
    };
}

//
// Created by egordm on 3-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>

using namespace litsignal::algorithm;

namespace litwidgets {
    struct WaveformContext : public AlgorithmContext<arma::fvec> {
        WaveformContext() : AlgorithmContext(arma::fvec2()) {}
    };

    class WaveformGenerator : public AlgorithmInterface<arma::fvec, WaveformContext> {
    public:
        WaveformGenerator() {}

        void processFrame(WaveformContext &context, arma::fvec &frame, int i) override {
            context.getOutput()(0) = abs(frame).max(); // Max value
            context.getOutput()(1) = sqrtf(arma::dot(frame, frame) / frame.size()); // RMS
        }

        WaveformContext createContext(arma::fvec &frame) override {
            return WaveformContext();
        }
    };

    using WaveformPipeline = AlgorithmPipeline<fvec, fvec, fvec, fmat, WaveformContext, WaveformGenerator>;
}

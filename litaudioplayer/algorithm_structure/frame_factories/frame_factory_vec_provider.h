//
// Created by egordm on 6-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <algorithm_structure/frame_factories/frame_factory_vec.h>
#include <providers/audio_provider_interface.h>
#include <structures/audio_buffer_deinterleaved_reference.h>

using namespace arma;

using namespace litsignal::algorithm;

namespace litaudioplayer { namespace algorithm {
    template<typename T>
    class FrameFactoryVecProvider : public FrameFactoryInterface<Col<T>>, public FrameHopInterface {
    private:
        const providers::AudioProviderInterface <T> *input;
        structures::AudioBufferDeinterleavedRef<T> buffer;
        uint processing_flags, channel;

    public:
        FrameFactoryVecProvider(const providers::AudioProviderInterface <T> *input, int frame_size, int hop_size,
                                uint channel = 0, uint processing_flags = 1, int buffer_size = 2048)
                : FrameHopInterface(frame_size, hop_size), input(input), buffer(1, buffer_size), channel(channel),
                  processing_flags(processing_flags) {}

        Col<T> create() override {
            return Col<T>(ACU(frame_size));
        }

        void fill(Col<T> &frame, int i) override {
            int out_count = 0;
            buffer.setChannel(0, &frame);
            input->request(&buffer, buffer.getSampleCount(), out_count, getPos(i), processing_flags);
        }

        virtual int getPos(int i) {
            return i * hop_size;
        }

        int getFrameCount() override {
            return static_cast<int>(std::floor(input->getSampleCount() / (float) hop_size));
        }

        int getInputSize() override {
            return input->getSampleCount();
        }
    };
}}

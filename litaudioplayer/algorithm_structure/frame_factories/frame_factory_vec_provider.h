//
// Created by egordm on 6-1-19.
//

#pragma once

#include <armadillo>
#include <algorithm_structure/algorithm_interface.h>
#include <algorithm_structure/frame_factories/frame_factory_vec.h>
#include <providers/audio_provider_interface.h>

using namespace arma;

using namespace litsignal::algorithm;

namespace litaudioplayer { namespace algorithm {
    template<typename T>
    class FrameFactoryVecProvider : public FrameFactoryInterface<Col<T>>, public FrameHopInterface {
    private:
        const providers::AudioProviderInterface <T> *input;
        litaudio::structures::AudioBufferDeinterleaved<T> buffer;
        uint processing_flags;

    public:
        FrameFactoryVecProvider(const providers::AudioProviderInterface <T> *input, int frame_size, int hop_size,
                                uint processing_flags = 1, int bufferSize = 2048)
                : FrameHopInterface(frame_size, hop_size), input(input), buffer(bufferSize, 1),
                  processing_flags(processing_flags) {}

        Col<T> create() override {
            return Col<T>(ACU(frame_size));
        }

        void fill(Col<T> &frame, int i) override {
            // TODO: make buffer also a deinterleaved feeder interface to directly write the frame but also support buffer
            int out_count = 0;
            int cursor = getPos(i);
            int out_cursor = 0;
            int remaining_samples = frame_size;
            while (remaining_samples > 0) {
                int req_count = std::min(buffer.getCapacity(), remaining_samples);
                input->request(&buffer, req_count, out_count, cursor, processing_flags);
                memcpy(frame.memptr() + out_cursor, buffer.getChannel(0), req_count * sizeof(T));
                cursor += buffer.getCapacity();
                out_cursor += buffer.getCapacity();
                remaining_samples -= buffer.getCapacity();
            }
        }

        virtual int getPos(int i) {
            return i * hop_size;
        }

        int getFrameCount() override {
            return static_cast<int>(std::round(input->getSampleCount() / (float) hop_size));
        }

        int getInputSize() override {
            return input->getSampleCount();
        }
    };
}}

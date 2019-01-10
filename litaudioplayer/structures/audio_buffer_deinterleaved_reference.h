//
// Created by egordm on 10-1-19.
//

#pragma once

#include <structures/audio_buffer_interface_deinterleaved.h>
#include <armadillo>

using namespace litaudio::structures;
using namespace arma;

namespace litaudioplayer { namespace structures {
    template<typename T>
    class AudioBufferDeinterleavedRef
            : public AudioBufferDeinterleavedInterface<T> {
    protected:
        std::vector<Col<T>*> buffers;

    public:
        AudioBufferDeinterleavedRef(int channel_count, int capacity)
                : AudioBufferDeinterleavedInterface<T>(channel_count, capacity), buffers() {
            buffers.resize(channel_count);
        }

        void setChannel(int channel, Col<T>* buffer) {
            buffers[channel] = buffer;
            this->capacity = ACI(buffer->size());
        }

        T *getChannel(int channel) override {
            return buffers[channel]->memptr();
        }

        void reset() override {
            for(auto buffer : buffers) buffer->fill(0);
        }
    };
}}

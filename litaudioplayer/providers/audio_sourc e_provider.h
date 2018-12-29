//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_container.h>
#include "audio_provider_interface.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace providers {
    template <typename T>
    class AudioSourceProvider : AudioProvider<T> {
    protected:
        std::shared_ptr<AudioContainer<T>> source;
        int cursor = 0;

    public:
        AudioSourceProvider(const std::shared_ptr<AudioContainer<T>> &source) : source(source) {}

        void request(AudioBufferDeinterleaved<T> *buffer, AudioBufferDeinterleaved<T> *swap, int sample_count,
                      int &out_sample_count) override {
            assert(buffer->getChannelCount() <= source->getChannelCount()); // Otherwise we need some split or average

            // Determine output sample count
            out_sample_count = min(sample_count, getSampleCount() - cursor);
            if(out_sample_count <= 0) return;

            // Copy the data
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                memcpy(buffer->getChannel(c), source->getData(c) + cursor, out_sample_count * sizeof(T));
            }
        }

        void reset() override {
            cursor = 0;
        }

        int getSampleCount() override {
            return source->getSampleCount();
        }

        int getCursor() override {
            return cursor;
        }

        void progress(int sample_count) override {
            cursor = min(cursor + sample_count, getSampleCount());
        }

        void setCursor(int value) override {
            cursor = value;
        }
    };
}}


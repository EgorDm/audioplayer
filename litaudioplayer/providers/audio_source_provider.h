#include <utility>

//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_container.h>
#include <cassert>
#include <memory>
#include "audio_provider_interface.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioSourceProvider : public AudioProviderInterface<T> {
    protected:
        std::shared_ptr<AudioContainerInterface> source;
        AudioBufferDeinterleavedInterface<T> *source_buffer;
        int cursor = 0;

    public:
        // TODO: restrict source to have deinterleaved buffer
        AudioSourceProvider(const std::shared_ptr<AudioContainerInterface> &source)
                : source(source),
                  source_buffer(dynamic_cast<AudioBufferDeinterleavedInterface<T>*>(source->getBuffer())) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count,
                     int cursor, uint processing_flags) const override {
            assert(buffer->getChannelCount() <= source->getChannelCount()); // Otherwise we need some split or average

            // Determine output sample count
            out_sample_count = std::min(sample_count, getSampleCount() - cursor);
            if (out_sample_count <= 0) return;

            // Copy the data
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                // TODO: remove the cast
                memcpy(buffer->getChannel(c), source_buffer->getChannel(c) + cursor, out_sample_count * sizeof(T));
            }
        }

        void reset() override {
            cursor = 0;
        }

        int getSampleCount() const override {
            return source->getSampleCount();
        }

        int getCursor() const override {
            return cursor;
        }

        void progress(int sample_count) override {
            cursor = std::min(cursor + sample_count, getSampleCount());
        }

        void setCursor(int value) override {
            cursor = value;
        }

        int getSampleRate() const override {
            return source->getSampleRate();
        }
    };
}}


//
// Created by egordm on 30-12-18.
//

#pragma once

#include "playback_interface.h"
#include "../providers/audio_provider_interface.h"

namespace litaudioplayer { namespace playback {
    template<typename T>
    class SimplePlayback : public PlaybackInterface<T> {
    protected:
        std::shared_ptr<providers::AudioProvider<T>> provider;
        float volume = 0.1f;

    public:
        SimplePlayback(const std::shared_ptr<providers::AudioProvider<T>> &provider) : provider(provider) {}

        void render(AudioBufferDeinterleaved<T> *buffer, int sample_count, int &out_sample_count) override {
            provider->request(buffer, buffer, sample_count, out_sample_count);

            // Apply volume
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                auto cbuffer = buffer->getChannel(c);
                for (int i = 0; i < out_sample_count; ++i) cbuffer[i] *= volume;
            }
        }

        void progress(int amount) override {
            provider->progress(amount);
        }
    };
}}
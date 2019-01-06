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
        std::shared_ptr<providers::AudioProviderInterface<T>> provider;
        float volume = 0.1f;

    public:
        SimplePlayback(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider = nullptr) : provider(provider) {}

        void request(AudioBufferDeinterleaved<T> *buffer, int sample_count, int &out_sample_count, int cursor) override {
            if(!provider) return;
            provider->request(buffer, sample_count, out_sample_count, cursor);

            // Apply volume
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                auto cbuffer = buffer->getChannel(c);
                for (int i = 0; i < out_sample_count; ++i) cbuffer[i] *= volume;
            }
        }

        void reset() override {
            if(provider) provider->reset();
        }

        void progress(int amount) override {
            provider->progress(amount);
        }

        void setCursor(int value) override {
            if(provider) provider->setCursor(value);
        }

        int getCursor() override {
            if(provider) return provider->getCursor();
            else return 0;
        }

        int getSampleCount() override {
            if(provider) return provider->getSampleCount();
            else return 0;
        }

        const std::shared_ptr<providers::AudioProviderInterface<T>> &getProvider() const {
            return provider;
        }

        void setProvider(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider) {
            SimplePlayback::provider = provider;
        }
    };
}}
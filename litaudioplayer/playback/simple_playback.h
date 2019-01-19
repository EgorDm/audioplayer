//
// Created by egordm on 30-12-18.
//

#pragma once

#include "playback_interface.h"
#include "../providers/audio_volume_processing_provider.h"

namespace litaudioplayer { namespace playback {
    template<typename T>
    class SimplePlayback : public PlaybackInterface<T> {
    protected:
        std::shared_ptr<providers::AudioVolumeProcessingProvider<T>> volume_processor;
        std::shared_ptr<providers::AudioProviderInterface<T>> &provider;

    public:
        explicit SimplePlayback(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider = nullptr)
                : volume_processor(std::make_shared<providers::AudioVolumeProcessingProvider<T>>(provider)),
                  provider(volume_processor->getChild()) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                int sample_count, int &out_sample_count, int cursor, uint processing_flags) const override {
            if (!provider) return;
            volume_processor->request(buffer, swap, sample_count, out_sample_count, cursor, processing_flags);
        }

        const std::shared_ptr<providers::AudioProviderInterface<T>> &getProvider() const override {
            return provider;
        }

        void setProvider(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider) override {
            SimplePlayback::provider = provider;
            PlaybackInterface<T>::setProvider(provider);
        }

        std::shared_ptr<providers::AudioVolumeProcessingProvider<T>> &getVolumeProcessor() {
            return volume_processor;
        }

        void reset() override {
            volume_processor->reset();
        }

        void progress(int amount) override {
            volume_processor->progress(amount);
        }

        void setCursor(int value) override {
            volume_processor->setCursor(value);
        }

        int getCursor() const override {
            return volume_processor->getCursor();
        }

        int getSampleCount() const override {
            return volume_processor->getSampleCount();
        }

        int getSampleRate() const override {
            return volume_processor->getSampleRate();
        }
    };
}}
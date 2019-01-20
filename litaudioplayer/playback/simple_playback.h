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

    public:
        explicit SimplePlayback(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider = nullptr)
                : volume_processor(std::make_shared<providers::AudioVolumeProcessingProvider<T>>(provider)) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                int sample_count, int &out_sample_count, int cursor, uint processing_flags) const override {
            volume_processor->request(buffer, swap, sample_count, out_sample_count, cursor, processing_flags);
        }

        std::shared_ptr<providers::AudioProviderInterface<T>> &getProvider() override {
            return volume_processor->getChild();
        }

        void setProvider(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider) override {
            volume_processor->setChild(provider);
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
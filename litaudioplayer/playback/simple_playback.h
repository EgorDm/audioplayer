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
        std::shared_ptr<providers::AudioVolumeProcessingProvider<T>> provider;

    public:
        explicit SimplePlayback(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider = nullptr)
            : provider(std::make_shared<providers::AudioVolumeProcessingProvider<T>>(provider)) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count, int cursor,
                uint processing_flags) const override {
            if(!provider) return;
            provider->request(buffer, sample_count, out_sample_count, cursor, processing_flags);
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

        int getCursor() const override {
            if(provider) return provider->getCursor();
            else return 0;
        }

        void setVolumeDb(float db) {
            provider->setVolumeDb(db);
        }

        float getVolumeDb() {
            return provider->getVolumeDb();
        }

        int getSampleCount() const override {
            if(provider) return provider->getSampleCount();
            else return 0;
        }

        const std::shared_ptr<providers::AudioProviderInterface<T>> &getProvider() const {
            return provider->getChild();
        }

        void setProvider(const std::shared_ptr<providers::AudioProviderInterface<T>> &provider) {
            SimplePlayback::provider->setChild(provider);
        }
    };
}}
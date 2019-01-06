//
// Created by egordm on 6-1-19.
//

#pragma once

#include "audio_processing_provider.h"

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioVolumeProcessingProvider : public AudioProcessingProvider<T> {
    protected:
        float volume = 0.1f;

        void process(AudioBufferDeinterleaved<T> *buffer, int sample_count) const override {
            // Apply volume
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                auto cbuffer = buffer->getChannel(c);
                for (int i = 0; i < sample_count; ++i) cbuffer[i] *= volume;
            }
        }

    public:
        AudioVolumeProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child, float volume = 0.1f)
                : AudioProcessingProvider<T>(child), volume(volume) {}

        uint getProcessingMask() const override {
            return 2 | AudioProcessingProvider<T>::getProcessingMask();
        }

        float getVolume() const {
            return volume;
        }

        void setVolume(float volume) {
            AudioVolumeProcessingProvider::volume = volume;
        }
    };
}}

//
// Created by egordm on 6-1-19.
//

#pragma once

#include "audio_processing_provider.h"
#include <iostream>

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioVolumeProcessingProvider : public AudioProcessingProvider<T> {
    protected:
        float volume;
        float thresh;

    public:
        AudioVolumeProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child, float volume = 0.5f,
                                      float thresh = -46)
                : AudioProcessingProvider<T>(child), volume(volume), thresh(powf(10, thresh / 20)) {}

        void process(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int cursor) const override {
            // Apply volume
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                auto cbuffer = buffer->getChannel(c);
                for (int i = 0; i < sample_count; ++i) cbuffer[i] *= volume;
            }
        }

        uint getProcessingMask() const override {
            return 2 | AudioProcessingProvider<T>::getProcessingMask();
        }

        float getVolume() const {
            return volume;
        }

        void setVolume(float p) {
            AudioVolumeProcessingProvider::volume = p;
            if (volume <= thresh) volume = 0;
        }

        void setVolumeDb(float db) {
            setVolume(powf(10, db / 20));
        }

        float getVolumeDb() {
            if (volume == 0) return -100;
            return 6.02f * log2(volume);
        }
    };
}}

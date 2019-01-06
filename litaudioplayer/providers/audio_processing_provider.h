//
// Created by egordm on 30-12-18.
//

#pragma once

#include "audio_provider_interface.h"

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioProcessingProvider : AudioProviderInterface<T> {
    protected:
        std::shared_ptr<AudioProviderInterface<T>> child = nullptr;

    private:
        void request(AudioBufferDeinterleaved<T> *buffer, int sample_count, int &out_sample_count) override {
            if(child) child->request(buffer, sample_count, out_sample_count);
            else out_sample_count = 0;
        }

        void reset() override {
            if (child) child->reset();
        }

        int getSampleCount() override {
            return child ? child->getSampleCount() : 0;
        }

        int getCursor() override {
            return child ? child->getCursor() : 0;
        }

        void progress(int sample_count) override {
            if (child) child->progress(sample_count);
        }

        void setCursor(int value) override {
            if (child) child->setCursor(value);
        }
    };
}}
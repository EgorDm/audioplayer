//
// Created by egordm on 30-12-18.
//

#pragma once

#include "audio_provider_interface.h"

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioProcessingProvider : public AudioProviderInterface<T> {
    protected:
        std::shared_ptr<AudioProviderInterface<T>> child = nullptr;

    protected:
        virtual void process(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count) const = 0;

    public:
        explicit AudioProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child) : child(child) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count,
                     int cursor, uint processing_flags) const override {
            if (child) child->request(buffer, sample_count, out_sample_count, cursor, processing_flags);
            else out_sample_count = 0;

            if(out_sample_count > 0 && (processing_flags & getProcessingMask()) == 0) {
                process(buffer, out_sample_count);
            }
        }

        void reset() override {
            if (child) child->reset();
        }

        int getSampleCount() const override {
            return child ? child->getSampleCount() : 0;
        }

        int getCursor() const override {
            return child ? child->getCursor() : 0;
        }

        void progress(int sample_count) override {
            if (child) child->progress(sample_count);
        }

        void setCursor(int value) override {
            if (child) child->setCursor(value);
        }

        const std::shared_ptr<AudioProviderInterface<T>> &getChild() const {
            return child;
        }

        void setChild(const std::shared_ptr<AudioProviderInterface<T>> &child) {
            AudioProcessingProvider::child = child;
        }

        virtual uint getProcessingMask() const {
            return 1;
        }
    };
}}
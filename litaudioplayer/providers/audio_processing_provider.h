//
// Created by egordm on 30-12-18.
//

#pragma once

#include "audio_provider_interface.h"

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioProcessingProvider : public AudioProviderInterface<T> {
    protected:
        std::shared_ptr<AudioProviderInterface < T>> child = nullptr;

    public:
        explicit AudioProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child) : child(child) {}

        virtual void process(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                             int sample_count, int cursor) const = 0;

        void request(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int &out_sample_count, int cursor, uint processing_flags) const override {
            if (getChild()) getChild()->request(buffer, swap, sample_count, out_sample_count, cursor, processing_flags);
            else out_sample_count = 0;

            if (out_sample_count > 0 && (processing_flags & getProcessingMask()) == 0) {
                process(buffer, swap, out_sample_count, cursor);
            }
        }

        void reset() override {
            if (getChild()) getChild()->reset();
        }

        int getSampleCount() const override {
            return getChild() ? getChild()->getSampleCount() : 0;
        }

        int getCursor() const override {
            return getChild() ? getChild()->getCursor() : 0;
        }

        void progress(int sample_count) override {
            if (getChild()) getChild()->progress(sample_count);
        }

        void setCursor(int value) override {
            if (getChild()) getChild()->setCursor(value);
        }

        virtual uint getProcessingMask() const {
            return 1;
        }

        int getSampleRate() const override {
            return getChild() ? getChild()->getSampleRate() : 1;
        }

        virtual const std::shared_ptr<AudioProviderInterface < T>> &getChild() const {
            return child;
        }

        virtual std::shared_ptr<AudioProviderInterface < T>> &getChild() {
            return child;
        }

        virtual void setChild(const std::shared_ptr<AudioProviderInterface < T>> &child) {
            AudioProcessingProvider::child = child;
        }
    };
}}
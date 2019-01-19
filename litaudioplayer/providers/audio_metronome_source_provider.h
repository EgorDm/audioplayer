//
// Created by egordm on 12-1-19.
//

#pragma once

#include <structs/time_signature.h>
#include "audio_source_provider.h"
#include "audio_metronome_processing_provider.h"

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioMetronomeSourceProvider : public AudioProviderInterface<T> {
    protected:
        AudioMetronomeProcessingProvider<T> processor;
        int cursor = 0;

    public:
        AudioMetronomeSourceProvider(const TimeSignature &time_signature,
                                     const std::shared_ptr<AudioContainerInterface> &downbeat_tick,
                                     const std::shared_ptr<AudioContainerInterface> &beat_tick)
                : processor(std::shared_ptr<AudioProviderInterface<T>>(this), time_signature, downbeat_tick,
                            beat_tick) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int &out_sample_count, int cursor, uint processing_flags) const override {
            buffer->reset();
            out_sample_count = sample_count;

            processor.process(buffer, out_sample_count, cursor);
        }

        void reset() override {
            cursor = 0;
        }

        int getSampleCount() const override {
            return static_cast<int>(processor.getTimeSignature().getBarDuration() * getSampleRate());
        }

        int getCursor() const override {
            return cursor;
        }

        void progress(int amount) override {
            this->cursor = (this->cursor + amount) % getSampleCount();
        }

        void setCursor(int value) override {
            cursor = value;
        }

        int getSampleRate() const override {
            return processor.getBeatTick()->getSampleRate();
        }
    };
}}
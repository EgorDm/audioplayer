#include <utility>

//
// Created by egordm on 18-1-19.
//

#pragma once

#include <structs/time_signature.h>
#include <utils/audio_buffer_utils.h>
#include "audio_processing_provider.h"

using namespace litaudioplayer::structs;

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioMetronomeProcessingProvider : public AudioProcessingProvider<T> {
    protected:
        TimeSignature time_signature;
        std::shared_ptr<AudioContainerDeinterleavedType<T>> downbeat_tick;
        std::shared_ptr<AudioContainerDeinterleavedType<T>> beat_tick;
        int beat_interval = 1;
        int bar_interval = 1;
        int offset = 0;

    public:
        AudioMetronomeProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child,
                                         const TimeSignature &time_signature,
                                         const std::shared_ptr<AudioContainerDeinterleavedType<T>> &downbeat_tick,
                                         const std::shared_ptr<AudioContainerDeinterleavedType<T>> &beat_tick)
                : AudioProcessingProvider<T>(child), time_signature(time_signature),
                  downbeat_tick(downbeat_tick), beat_tick(beat_tick) {
            setTimeSignature(time_signature);
        }

        void process(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int cursor) const override {
            int out_cursor = 0;
            int main_cursor = cursor + offset;
            int in_cursor = main_cursor % beat_interval;
            int beat_id = 0;
            AudioBufferDeinterleavedInterface<T> *tick_buffer;
            while (out_cursor < sample_count) {
                beat_id = (main_cursor % bar_interval) / beat_interval;
                tick_buffer = beat_id == 0 ? downbeat_tick->getTypedBuffer() : beat_tick->getTypedBuffer();
                int copy_count = std::min(tick_buffer->getSampleCount() - in_cursor, sample_count - out_cursor);
                litaudio::utils::add_buffers<T>(buffer, tick_buffer, out_cursor, in_cursor, copy_count);
                out_cursor += beat_interval - in_cursor;
                main_cursor += beat_interval - in_cursor;
                in_cursor = 0;
            }
        }

        const TimeSignature &getTimeSignature() const {
            return time_signature;
        }

        void setTimeSignature(const TimeSignature &time_signature) {
            AudioMetronomeProcessingProvider::time_signature = time_signature;
            // Interval between each 1 / ts.upper th note. In samples
            beat_interval = static_cast<int>(this->getSampleRate() * time_signature.getBeatDuration());
            bar_interval = static_cast<int>(this->getSampleRate() * time_signature.getBarDuration());
            offset = static_cast<int>(time_signature.getOffset() * this->getSampleRate());
            while (offset > 0) offset -= beat_interval;
        }

        const std::shared_ptr<AudioContainerInterface> &getDownbeatTick() const {
            return downbeat_tick;
        }

        void setDownbeatTick(const std::shared_ptr<AudioContainerInterface> &downbeat_tick) {
            AudioMetronomeProcessingProvider::downbeat_tick = downbeat_tick;
        }

        const std::shared_ptr<AudioContainerDeinterleavedType<T>> &getBeatTick() const {
            return beat_tick;
        }

        void setBeatTick(const std::shared_ptr<AudioContainerDeinterleavedType<T>> &beat_tick) {
            AudioMetronomeProcessingProvider::beat_tick = beat_tick;
        }
    };
}}
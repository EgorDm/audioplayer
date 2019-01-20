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

    public:
        AudioMetronomeProcessingProvider(const std::shared_ptr<AudioProviderInterface<T>> &child,
                                         const TimeSignature &time_signature,
                                         const std::shared_ptr<AudioContainerDeinterleavedType<T>> &downbeat_tick,
                                         const std::shared_ptr<AudioContainerDeinterleavedType<T>> &beat_tick)
                : AudioProcessingProvider<T>(child), time_signature(time_signature),
                  downbeat_tick(downbeat_tick), beat_tick(beat_tick) {}

        void process(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int cursor) const override {
            // Interval between each 1 / ts.upper th note. In samples
            int interval = static_cast<int>(this->getSampleRate() * time_signature.getBeatDuration());
            int offset = static_cast<int>(time_signature.getOffset() * this->getSampleRate());

            int out_cursor = 0;
            int in_cursor = (cursor + offset) % interval;
            while (out_cursor < sample_count) {
                int copy_count = std::min(beat_tick->getSampleCount() - in_cursor, sample_count - out_cursor);
                litaudio::utils::add_buffers<T>(buffer, beat_tick->getTypedBuffer(), out_cursor, in_cursor, copy_count);
                out_cursor += interval - in_cursor;
                in_cursor = 0;
            }
        }

        const TimeSignature &getTimeSignature() const {
            return time_signature;
        }

        void setTimeSignature(const TimeSignature &time_signature) {
            AudioMetronomeProcessingProvider::time_signature = time_signature;
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
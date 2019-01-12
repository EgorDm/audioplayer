//
// Created by egordm on 12-1-19.
//

#pragma once

#include "audio_source_provider.h"

namespace litaudioplayer { namespace providers {
    struct TimeSignature {
        float bpm, offset;
        int upper, lower;

        TimeSignature(float bpm, float offset, int upper, int lower)
                : bpm(bpm), offset(offset), upper(upper), lower(lower) {}
    };

    template<typename T>
    class AudioMetronomeSourceProvider : public AudioSourceProvider<T> {
    protected:
        int sample_rate;
        TimeSignature ts;

    public:
        AudioMetronomeSourceProvider(const std::shared_ptr<AudioContainerInterface> &source, int sample_rate,
                                     const TimeSignature &ts)
                : AudioSourceProvider<T>(source), sample_rate(sample_rate), ts(ts) {}

        void request(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count, int cursor,
                     uint processing_flags) const override {
            buffer->reset();
            out_sample_count = sample_count;
            // Interval between each 1 / ts.upper th note. In samples
            int interval = static_cast<int>((1.f / ts.bpm * 60) / (ts.upper / 4.f) * sample_rate);
            int offset = static_cast<int>(ts.offset * sample_rate);

            int out_cursor = 0;
            int in_cursor = (cursor + offset) % interval;
            while (out_cursor < sample_count) {
                int copy_count = std::min(this->source_buffer->getSampleCount() - in_cursor, sample_count - out_cursor);
                for (int i = 0; i < copy_count; ++i) {
                    for (int c = 0; c < buffer->getChannelCount(); ++c) {
                        *(buffer->getChannel(c) + out_cursor + i) = *(this->source_buffer->getChannel(c) + in_cursor + i);
                    }
                }

                out_cursor += interval - in_cursor;
                in_cursor = 0;
            }
        }

        int getSampleCount() const override {
            return AudioSourceProvider<T>::getSampleCount() * 10;
        }

        int getSampleRate() const {
            return sample_rate;
        }

        void setSampleRate(int sample_rate) {
            AudioMetronomeSourceProvider::sample_rate = sample_rate;
        }

        const TimeSignature &getTs() const {
            return ts;
        }

        void setTs(const TimeSignature &ts) {
            AudioMetronomeSourceProvider::ts = ts;
        }
    };

}}
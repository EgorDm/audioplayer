//
// Created by egordm on 18-1-19.
//

#pragma once

namespace litaudioplayer { namespace structs {
    class TimeSignature {
        float bpm, offset;
        int upper, lower;
        float beat_duration, bar_duration;

    public:
        TimeSignature(float bpm, float offset, int upper, int lower)
                : bpm(bpm), offset(offset), upper(upper), lower(lower) {
            calculateDurations();
        }

        void calculateDurations() {
            beat_duration = (1.f / bpm * 60) * (4.f / (float) lower);
            bar_duration = beat_duration * upper;
        }

        float getBpm() const {
            return bpm;
        }

        void setBpm(float bpm) {
            TimeSignature::bpm = bpm;
            calculateDurations();
        }

        float getOffset() const {
            return offset;
        }

        void setOffset(float offset) {
            TimeSignature::offset = offset;
        }

        int getUpper() const {
            return upper;
        }

        void setUpper(int upper) {
            TimeSignature::upper = upper;
            calculateDurations();
        }

        int getLower() const {
            return lower;
        }

        void setLower(int lower) {
            TimeSignature::lower = lower;
            calculateDurations();
        }

        float getBeatDuration() const {
            return beat_duration;
        }

        float getBarDuration() const {
            return bar_duration;
        }
    };
}}

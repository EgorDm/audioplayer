//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_buffer_deinterleaved.h>

using namespace litaudio::structures;

namespace litaudioplayer { namespace providers {
    template<typename T>
    class AudioProvider {
    public:
        virtual ~AudioProvider() = default;

        virtual void request(AudioBufferDeinterleaved<T> *buffer, AudioBufferDeinterleaved<T> *swap,
                              int sample_count, int &out_sample_count) = 0;

        virtual void reset() = 0;

        virtual int getSampleCount() = 0;

        virtual int getCursor() = 0;

        virtual void progress(int sample_count) = 0;

        virtual void setCursor(int value) = 0;

        bool ended() {
            return getCursor() >= getSampleCount();
        }
    };
}}

//
// Created by egordm on 30-12-18.
//

#pragma once

#include <structures/audio_buffer_deinterleaved.h>
#include "playback_listener.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace playback {

    enum PlaybackStatus {
        Stopped,
        Playing,
        Paused
    };

    template <typename T>
    class PlaybackInterface : public PlaybackListener {
    protected:
        PlaybackStatus status;

    public:
        virtual void render(AudioBufferDeinterleaved<T> *buffer, int sample_count, int &out_sample_count) = 0;

        virtual void progress(int amount) = 0;

        PlaybackStatus getStatus() const {
            return status;
        }

        void on_start() override {
            status = Playing;
        }

        void on_pause() override {
            status = Paused;
        }

        void on_stop() override {
            status = Stopped;
        }
    };

}}

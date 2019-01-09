//
// Created by egordm on 30-12-18.
//

#pragma once

#include <litsignal_constants.h>
#include "playback_listener.h"
#include "../providers/audio_provider_interface.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace playback {

    enum PlaybackStatus {
        Stopped,
        Playing,
        Paused
    };

    template<typename T>
    class PlaybackInterface : public providers::AudioProviderInterface<T>, public PlaybackListener {
    protected:
        PlaybackStatus status = Stopped;

    public:
        PlaybackInterface() = default;

        PlaybackStatus getStatus() const {
            return status;
        }

        void onStart() override {
            status = Playing;
        }

        void onPause() override {
            status = Paused;
        }

        void onStop() override {
            status = Stopped;
        }

        void onSeek(float p) override {
            this->setCursor(ACI(p * this->getSampleCount()));
        }
    };

}}

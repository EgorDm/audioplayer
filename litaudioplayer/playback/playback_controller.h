//
// Created by egordm on 30-12-18.
//

#pragma once

#include <unordered_set>
#include <utils/debug.h>
#include "playback_listener.h"

#define PlaybackController_TAG "PlaybackController"

using namespace litcore;

namespace litaudioplayer { namespace playback {
    class PlaybackController {
    private:
        std::unordered_set<PlaybackListener*> listeners;

    public:
        void addListener(PlaybackListener* listener) {
            if(listener == nullptr) return;
            listeners.insert(listener);
        }

        void removeRistener(PlaybackListener* listener) {
            listeners.erase(listener);
        }

        void start() {
            debug::log(PlaybackController_TAG, "Starting playback");
            for(auto listener : listeners) listener->onStart();
        }

        void pause() {
            debug::log(PlaybackController_TAG, "Pausing playback");
            for(auto listener : listeners) listener->onPause();
        }

        void seek(float p) {
            debug::log(PlaybackController_TAG, "Seeking playback position");
            for(auto listener : listeners) listener->onSeek(p);
        }

        void reset() {
            debug::log(PlaybackController_TAG, "Resetting playback");
            for(auto listener : listeners) listener->onReset();
        }

        void stop() {
            debug::log(PlaybackController_TAG, "Stopping playback");
            for(auto listener : listeners) listener->onStop();
        }
    };
}}

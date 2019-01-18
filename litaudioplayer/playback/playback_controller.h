//
// Created by egordm on 30-12-18.
//

#pragma once

#include <unordered_set>
#include <utils/debug.h>
#include <helpers/observable.h>
#include "playback_listener.h"

#define PlaybackController_TAG "PlaybackController"

using namespace litcore;

namespace litaudioplayer { namespace playback {
    class PlaybackController : public helpers::Observable<PlaybackListener> {
    public:
        void start() {
            debug::log(PlaybackController_TAG, "Starting playback");
            EACH_OBSERVER(observer->onStart());
        }

        void pause() {
            debug::log(PlaybackController_TAG, "Pausing playback");
            EACH_OBSERVER(observer->onPause());
        }

        void seek(float p) {
            //debug::log(PlaybackController_TAG, "Seeking playback position"); TODO: is bit spammy
            EACH_OBSERVER(observer->onSeek(p));
        }

        void reset() {
            debug::log(PlaybackController_TAG, "Resetting playback");
            EACH_OBSERVER(observer->onReset());
        }

        void stop() {
            debug::log(PlaybackController_TAG, "Stopping playback");
            EACH_OBSERVER(observer->onStop());
        }
    };
}}

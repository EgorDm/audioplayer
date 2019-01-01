//
// Created by egordm on 30-12-18.
//

#pragma once

namespace litaudioplayer { namespace playback {
    class PlaybackListener {
    public:
        virtual void onStart() {};

        virtual void onPause() {};

        virtual void onSeek(float p) {};

        virtual void onReset() {};

        virtual void onStop() {};
    };
}}

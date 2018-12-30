//
// Created by egordm on 30-12-18.
//

#pragma once

namespace litaudioplayer { namespace playback {
    class PlaybackListener {
    public:
        virtual void on_start() {};

        virtual void on_pause() {};

        virtual void on_seek(float p) {};

        virtual void on_reset() {};

        virtual void on_stop() {};
    };
}}

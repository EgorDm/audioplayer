//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_buffer_deinterleaved.h>
#include "../engine_properties.h"
#include "../playback/playback_listener.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace drivers {
    template <typename T>
    class DriverInterface : public playback::PlaybackListener {
    protected:
        bool initialized = false;

    public:
        virtual bool create(EngineProperties &properties) = 0;

        virtual void destroy() = 0;

        virtual void render() = 0;

        virtual int write(const AudioBufferDeinterleaved<T> *buffer, int sample_count) = 0;

        virtual bool isInitialized() {
            return initialized;
        }
    };
}}

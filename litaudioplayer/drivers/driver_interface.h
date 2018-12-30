//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_buffer_deinterleaved.h>
#include <playback/playback_interface.h>
#include "../engine_properties.h"
#include "../playback/playback_listener.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace drivers {
    template<typename T>
    class DriverInterface : public playback::PlaybackListener {
    protected:
        bool initialized = false;
        std::shared_ptr<AudioBufferDeinterleaved<T>> buffer = nullptr;
        playback::PlaybackInterface<T> *playback = nullptr;

    public:
        DriverInterface(playback::PlaybackInterface<T> *playback) : playback(playback) {}

        virtual bool create(EngineProperties &properties) {
            buffer = std::make_shared<AudioBufferDeinterleaved<float>>(properties.buffer_size,
                                                                       properties.channel_count);
            return true;
        }

        virtual void destroy() = 0;

        virtual int render(int sample_count) = 0;

        virtual bool isInitialized() {
            return initialized;
        }

        playback::PlaybackInterface<T> *getPlayback() const {
            return playback;
        }

        void setPlayback(playback::PlaybackInterface<T> *playback) {
            DriverInterface::playback = playback;
        }
    };
}}

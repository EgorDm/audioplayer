//
// Created by egordm on 29-12-18.
//

#pragma once

#include <playback/playback_interface.h>
#include <memory>
#include "../engine_properties.h"
#include "playback/playback_controller.h"

using namespace litaudio::structures;

namespace litaudioplayer { namespace drivers {
    template<typename T>
    class DriverInterface : public playback::PlaybackControllerObserver {
    protected:
        bool initialized = false;
        std::unique_ptr<AudioBufferDeinterleaved<T>> buffer = nullptr;
        std::unique_ptr<AudioBufferDeinterleaved<T>> swap = nullptr;
        std::shared_ptr<playback::PlaybackInterface<T>> playback = nullptr;

    public:
        explicit DriverInterface(const std::shared_ptr<playback::PlaybackInterface<T>> &playback)
                : playback(playback) {}

        virtual bool create(EngineProperties &properties) {
            buffer = std::make_unique<AudioBufferDeinterleaved<T>>(properties.channel_count, properties.buffer_size);
            // TODO: cap the swap buffer size
            swap = std::make_unique<AudioBufferDeinterleaved<T>>(properties.channel_count, properties.buffer_size);
            return true;
        }

        virtual void destroy() = 0;

        virtual int render(int sample_count) = 0;

        virtual bool isInitialized() {
            return initialized;
        }

        std::shared_ptr<playback::PlaybackInterface<T>> &getPlayback() {
            return playback;
        }

        void setPlayback(const std::shared_ptr<playback::PlaybackInterface<T>> &playback) {
            DriverInterface::playback = playback;
        }
    };
}}

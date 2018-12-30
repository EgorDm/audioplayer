//
// Created by egordm on 30-12-18.
//

#pragma once

#include "playback/playback_listener.h"
#include "drivers/driver_interface.h"
#include "playback/playback_controller.h"
#include "playback/playback_interface.h"

#define AudioEngine_TAG "AudioEngine"

namespace litaudioplayer {
    template<typename T>
    class AudioEngine : public playback::PlaybackListener {
    private:
        EngineProperties properties;
        std::shared_ptr<drivers::DriverInterface<T>> driver = nullptr;
        std::shared_ptr<playback::PlaybackController> controller = nullptr;
        std::shared_ptr<playback::PlaybackInterface<T>> playback = nullptr;

    public:
        AudioEngine(const EngineProperties &properties, const std::shared_ptr<drivers::DriverInterface<T>> &driver,
                    const std::shared_ptr<playback::PlaybackInterface<T>> &playback)
                : properties(properties), driver(driver), playback(playback) {

            // Create playback controller and register listeners
            controller = std::make_shared<playback::PlaybackController>();
            controller->addListener(this);
            controller->addListener(playback);
            controller->addListener(driver);
        }
    };
}

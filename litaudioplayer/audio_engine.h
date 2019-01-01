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
        std::unique_ptr<playback::PlaybackController> controller = nullptr;
        std::shared_ptr<playback::PlaybackInterface<T>> playback = nullptr;

    public:
        AudioEngine(EngineProperties &properties, const std::shared_ptr<drivers::DriverInterface<T>> &driver,
                    const std::shared_ptr<playback::PlaybackInterface<T>> &playback)
                : properties(properties), driver(driver), playback(playback) {

            driver->setPlayback(playback);

            // Create playback controller and register listeners
            controller = std::make_unique<playback::PlaybackController>();
            controller->addListener(this);
            controller->addListener(playback.get());
            controller->addListener(driver.get());
        }

        const std::unique_ptr<playback::PlaybackController> &getController() const {
            return controller;
        }

        const std::shared_ptr<playback::PlaybackInterface<T>> &getPlayback() const {
            return playback;
        }

        void setPlayback(const std::shared_ptr<playback::PlaybackInterface<T>> &playback) {
            AudioEngine::playback = playback;
            driver->setPlayback(playback);
        }
    };

    template<typename T, typename D>
    AudioEngine<T> *
    create_engine(EngineProperties &properties, const std::shared_ptr<playback::PlaybackInterface<T>> &playback) {
        auto driver = std::make_shared<D>(playback);
        LIT_ASSERT(driver->create(properties), AudioEngine_TAG, "Failed creating the audio driver!", nullptr);
        return new AudioEngine<T>(properties, driver, playback);
    }


}

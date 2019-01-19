//
// Created by egordm on 30-12-18.
//

#pragma once

#include <litsignal_constants.h>
#include "playback_controller.h"
#include "../providers/audio_provider_interface.h"

using namespace litaudio::structures;
using namespace litaudioplayer::providers;

namespace litaudioplayer { namespace playback {

    enum PlaybackStatus {
        NotReady,
        Ready,
        Stopped,
        Playing,
        Paused
    };

    template<typename T>
    struct PlaybackObserver {
        virtual void onProviderChange(const std::shared_ptr<AudioProviderInterface<T>> &provider) = 0;
    };

    template<typename T>
    class PlaybackInterface
            : public AudioProviderInterface<T>, public helpers::Observable<PlaybackObserver<T>>,
              public PlaybackControllerObserver {
    protected:
        PlaybackStatus status = Stopped;

    public:
        using ObserverType = PlaybackObserver<T>;

        PlaybackInterface() = default;

        virtual const std::shared_ptr<AudioProviderInterface<T>> &getProvider() const = 0;

        virtual void setProvider(const std::shared_ptr<AudioProviderInterface<T>> &provider) {
            EACH_OBSERVER(observer->onProviderChange(provider));
        };

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

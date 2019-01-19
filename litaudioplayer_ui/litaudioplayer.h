//
// Created by egordm on 18-1-19.
//

#pragma once

#include <audio_engine.h>
#include <playback/simple_playback.h>
#include <playback/litaudioplayback.h>

namespace litaudioplayer {
    class LitAudioplayer : public playback::PlaybackControllerObserver, public playback::LitAudioPlayback::Loader {
    protected:
        std::unique_ptr<AudioEngine<float>> engine;
        std::shared_ptr<playback::LitAudioPlayback> playback;

    public:
        LitAudioplayer();

        bool addAudioFile(const std::string &path);

        std::shared_ptr<playback::LitAudioPlayback> &getPlayback();

        std::unique_ptr<AudioEngine<float>> &getEngine();

        playback::PlaybackQueue<structs::AudioItemDescriptor> &getQueue();

        std::shared_ptr<AudioProviderInterface<float>> load(structs::AudioItemDescriptor *item) override;
    };
}


//
// Created by egordm on 18-1-19.
//

#pragma once

#include <structs/audio_item_descriptor.h>
#include <playback/queued_playback.h>
#include <providers/audio_mixer_processing_provider.h>
#include <providers/audio_metronome_source_provider.h>
#include <engine_properties.h>

using namespace litaudioplayer::providers;

namespace litaudioplayer { namespace playback {
    class LitAudioPlayback : public QueuedPlayback<float, structs::AudioItemDescriptor> {
    protected:
        std::shared_ptr<AudioMixerProcessingProvider<float>> mixer_provider;
        std::shared_ptr<AudioMetronomeSourceProvider<float>> metronome_provider;

    public:
        LitAudioPlayback(const EngineProperties &properties, AudioProviderLoader<float, structs::AudioItemDescriptor *> *loader);

        std::shared_ptr<AudioMixerProcessingProvider<float>> &getMixerProvider();

        std::shared_ptr<AudioMetronomeSourceProvider<float>> &getMetronomeProvider();

        std::shared_ptr<AudioProviderInterface<float>> &getProvider() override;

        void setProvider(const std::shared_ptr<AudioProviderInterface<float>> &provider) override;

        void setTimeSignature(const structs::TimeSignature &time_signature);
    };
}}


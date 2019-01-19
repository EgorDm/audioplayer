//
// Created by egordm on 18-1-19.
//

#pragma once

#include <structs/audio_item_descriptor.h>
#include <playback/queued_playback.h>
#include <providers/audio_mixer_processing_provider.h>
#include <engine_properties.h>

using namespace litaudioplayer::providers;

namespace litaudioplayer { namespace playback {
    class LitAudioPlayback : public QueuedPlayback<float, structs::AudioItemDescriptor> {
    protected:
        std::shared_ptr<AudioMixerProcessingProvider<float>> mixer_provider;
    public:
        LitAudioPlayback(const EngineProperties &properties, AudioProviderLoader<float, structs::AudioItemDescriptor *> *loader);
    };
}}


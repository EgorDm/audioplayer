//
// Created by egordm on 18-1-19.
//

#include "litaudioplayback.h"


using namespace litaudioplayer::playback;
using namespace litaudioplayer::structs;


LitAudioPlayback::LitAudioPlayback(const EngineProperties &properties,
                                   AudioProviderLoader<float, AudioItemDescriptor *> *loader)
        : QueuedPlayback(loader),
          mixer_provider(std::make_shared<AudioMixerProcessingProvider<float>>(2, properties.channel_count,
                                                                               properties.buffer_size)) {
    provider = mixer_provider->getMaster();
}

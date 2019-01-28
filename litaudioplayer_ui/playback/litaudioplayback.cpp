//
// Created by egordm on 18-1-19.
//

#include <audiofile/simplified.h>
#include "litaudioplayback.h"


using namespace litaudioplayer::playback;
using namespace litaudioplayer::structs;
using namespace litaudiofile::simplified;


LitAudioPlayback::LitAudioPlayback(const EngineProperties &properties,
                                   AudioProviderLoader<float, AudioItemDescriptor *> *loader)
        : QueuedPlayback(loader),
          mixer_provider(std::make_shared<AudioMixerProcessingProvider<float>>(2, properties.channel_count,
                                                                               properties.buffer_size)),
          metronome_provider(nullptr) {
    getVolumeProcessor()->setChild(mixer_provider);

    // Loading metronome
    auto downbeat_tick = std::shared_ptr<AudioContainerDeinterleavedType<float>>(
            read_audio<float>("data/metronomes/A/MetronomeUp.wav", properties.sample_rate));
    auto beat_tick = std::shared_ptr<AudioContainerDeinterleavedType<float>>(
            read_audio<float>("data/metronomes/A/Metronome.wav", properties.sample_rate));
    metronome_provider = std::make_shared<AudioMetronomeSourceProvider<float>>(TimeSignature(), downbeat_tick,
                                                                               beat_tick);
    mixer_provider->setChannel(METRONOME_MIX_INDEX, metronome_provider);

}

std::shared_ptr<AudioMixerProcessingProvider<float>> &LitAudioPlayback::getMixerProvider() {
    return mixer_provider;
}

std::shared_ptr<AudioMetronomeSourceProvider<float>> &LitAudioPlayback::getMetronomeProvider() {
    return metronome_provider;
}

std::shared_ptr<AudioProviderInterface<float>> &LitAudioPlayback::getProvider() {
    return mixer_provider->getChild();
}

void LitAudioPlayback::setProvider(const std::shared_ptr<AudioProviderInterface<float>> &provider) {
    mixer_provider->setChannel(AUDIO_MIX_INDEX, provider);
    EACH_OBSERVER(observer->onProviderChange(provider));
}

void LitAudioPlayback::setTimeSignature(const TimeSignature &time_signature) {
    getMetronomeProvider()->getProcessor().setTimeSignature(time_signature);
}



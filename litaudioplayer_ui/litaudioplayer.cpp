//
// Created by egordm on 18-1-19.
//

#include <drivers/portaudio_driver.h>
#include "litaudioplayer.h"
#include <audiofile/reading.h>
#include <providers/audio_source_provider.h>
#include <utils/audio_info.h>

using namespace litaudioplayer;
using namespace litaudio::structures;
using namespace litaudiofile;

LitAudioplayer::LitAudioplayer() {
    EngineProperties properties(2, 44100, 2048);
    playback = std::make_shared<playback::LitAudioPlayback>(properties, this);
    engine = std::unique_ptr<AudioEngine<float>>(create_engine<float, drivers::PortAudioDriver>(properties, playback));
    engine->getController()->addObserver(this);
}

bool LitAudioplayer::addAudioFile(const std::string &path) {
    auto item = new structs::AudioItemDescriptor(utils::getAudioFileTitle(path), true, path);
    getPlayback()->getQueue().enqueue(item);
    return true;
}

std::shared_ptr<AudioProviderInterface<float>>
LitAudioplayer::load(structs::AudioItemDescriptor *item) {
    if(!item->has_audio) return nullptr;

    auto src = std::make_shared<AudioContainerDeinterleaved<float>>();
    src->setSampleRate(engine->getProperties().sample_rate);
    AudioReader reader(src.get(), item->path);
    if(!reader.read()) return nullptr;

    return std::make_shared<providers::AudioSourceProvider<float>>(src);
}

std::shared_ptr<playback::LitAudioPlayback> &LitAudioplayer::getPlayback() {
    return playback;
}

std::unique_ptr<AudioEngine<float>> &LitAudioplayer::getEngine() {
    return engine;
}

playback::PlaybackQueue<structs::AudioItemDescriptor> &LitAudioplayer::getQueue() {
    return playback->getQueue();
}

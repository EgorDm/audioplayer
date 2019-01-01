//
// Created by egordm on 29-12-18.
//

#include <QtWidgets/QApplication>
#include <iostream>
#include <structures/audio_container.h>
#include <audiofile/reading.h>
#include <audio_engine.h>
#include <playback/simple_playback.h>
#include "drivers/portaudio_driver.h"
#include "providers/audio_source_provider.h"

using namespace litaudiofile;
using namespace litaudioplayer;
using namespace litaudio::structures;

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    std::cout << "Hello, World!" << std::endl;

    auto src = std::make_shared<AudioContainer<float>>(AV_SAMPLE_FMT_FLTP);
    AudioReader reader(src.get(), "data/hangar.mp3");
    if(!reader.read()) exit(1);

    EngineProperties properties(src->getChannelCount(), src->getSampleRate(), 2048);
    auto provider = std::make_shared<providers::AudioSourceProvider<float>>(src);
    auto playback = std::make_shared<playback::SimplePlayback<float>>(provider);
    auto driver = std::make_shared<drivers::PortAudioDriver>(playback);
    LIT_ASSERT(driver->create(properties), "Main", "Failed creating driver", 1);

    AudioEngine<float> engine(properties, driver, playback);
    engine.getController()->start();

    return application.exec();
}
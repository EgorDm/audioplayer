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
#include "player_window.h"
#include "test_window.h"

using namespace litaudiofile;
using namespace litaudioplayer;
using namespace litaudio::structures;

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

   // PlayerWindow window;
    TestWindow window;
    window.show();

    return application.exec();
}
//
// Created by egordm on 18-1-19.
//

#pragma once

#include <string>
#include <structs/time_signature.h>

namespace litaudioplayer { namespace structs {
    struct AudioItemDescriptor {
        std::string title;

        bool has_audio;
        std::string path;

        int uid;

        bool has_metronome;
        TimeSignature metronome_data;

        AudioItemDescriptor(const std::string &title, bool has_audio, const std::string &path,
                            bool has_metronome = false, const TimeSignature &metronome_data = TimeSignature())
                : title(title), has_audio(has_audio), path(path), has_metronome(has_metronome),
                  metronome_data(metronome_data), uid(rand()) {}
    };
}}

//
// Created by egordm on 1-1-19.
//

#pragma once

#include <string>
#include <audiofile/tag_reader.h>

using namespace litaudiofile;

enum AudioType {
    AudioFile
};

struct AudioItem {
    AudioType type;
    std::string path;
    std::string title;
};

inline AudioItem create_audio_item(std::string path) {
    AudioItem ret;
    ret.type = AudioFile;
    ret.path = path;
    ret.title = path;

    if(!path.empty()) {
        TagReader reader(path);
        auto tags = reader.getTags();

        ret.title = "";
        if (!tags->artist().isEmpty()) {
            ret.title += tags->artist().toCString();
            ret.title += " - ";
        }
        ret.title += tags->title().toCString();
    }

    return ret;
}
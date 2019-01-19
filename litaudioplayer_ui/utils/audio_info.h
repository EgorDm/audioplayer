//
// Created by egordm on 19-1-19.
//

#pragma once

#include <string>
#include <audiofile/tag_reader.h>

namespace litaudioplayer { namespace utils {
    std::string getAudioFileTitle(const std::string &path) {
        TagReader reader(path);
        auto tags = reader.getTags();

        std::string title = "";
        if (!tags->artist().isEmpty()) {
            title += tags->artist().toCString();
            title += " - ";
        }
        title += tags->title().toCString();

        if(title.empty()) title = path;
        return title;
    }
}}

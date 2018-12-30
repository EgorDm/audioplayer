//
// Created by egordm on 30-12-18.
//

#pragma once

namespace litaudioplayer {
    struct EngineProperties {
        int channel_count;
        int sample_rate;
        int buffer_size;

        EngineProperties(int channel_count, int sample_rate, int buffer_size)
                : channel_count(channel_count), sample_rate(sample_rate), buffer_size(buffer_size) {}
    };
}
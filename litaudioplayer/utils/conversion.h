//
// Created by egordm on 1-1-19.
//

#pragma once

using namespace litaudio::structures;
using namespace lit::math;

namespace litaudioplayer { namespace utils {
    template <typename FLT, typename FIX>
    void convert_ifltp_dfixp(AudioBufferDeinterleaved<FLT> *src, FIX* dst, int count) {
        if(count == 0) return;

        int max_cursor = count * src->getChannelCount();
        int channel_count = src->getChannelCount();
        int out_cursor, in_cursor;
        FLT *c_buffer;
        for (int c = 0; c < src->getChannelCount(); ++c) {
            c_buffer = src->getChannel(c);
            for (out_cursor = c, in_cursor = 0; out_cursor < max_cursor; out_cursor += channel_count) {
                dst[out_cursor] = (uint16_t) (clip(c_buffer[in_cursor++], -1, 1) * 32760);
            }
        }
    }
}}
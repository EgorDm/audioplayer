//
// Created by egordm on 29-12-18.
//

#pragma once

#include <structures/audio_buffer_interface_deinterleaved.h>

using namespace litaudio::structures;

namespace litaudioplayer { namespace providers {
    const uint PROCESS_ALL = 0;

    template<typename T>
    class AudioProviderInterface {
    public:
        virtual ~AudioProviderInterface() = default;

        void requestCurrent(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count,
                     uint processing_flags = PROCESS_ALL) const {
            request(buffer, sample_count, out_sample_count, getCursor(), processing_flags);
        }

        virtual void request(AudioBufferDeinterleavedInterface<T> *buffer, int sample_count, int &out_sample_count,
                             int cursor, uint processing_flags) const = 0;

        virtual void reset() = 0;

        virtual int getSampleCount() const = 0;

        virtual int getCursor() const = 0;

        virtual void progress(int amount) = 0;

        virtual void setCursor(int value) = 0;

        bool ended() {
            return getCursor() >= getSampleCount();
        }
    };
}}

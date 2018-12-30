//
// Created by egordm on 30-12-18.
//

#pragma once

#include <structures/pcm_container.h>
#include <utils/debug.h>
#include <audiofile/writing.h>
#include "driver_interface.h"
#include "../audioplayer_defines.h"

#define FileDriver_TAG "FileDriver"

using namespace litcore;
using namespace litaudiofile;

namespace litaudioplayer { namespace drivers {
    class FileDriver : DriverInterface<float> {
    protected:
        litaudio::structures::PCMAudio output;

    public:
    private:
        bool create(EngineProperties &properties) override {
            debug::log(FileDriver_TAG, "Creating FileDriver");

            output = PCMAudio(properties.channel_count);
            output.setSampleRate(properties.sample_rate);

            initialized = true;
            return true;
        }

        void destroy() override {
            debug::log(FileDriver_TAG, "Destroying FileDriver");

            AudioWriter writer(&output, FILE_DRIVER_SAVE_PATH);
            if (!writer.write()) {
                debug::log_error(FileDriver_TAG, "FileDriver could not save the audio stream.");
            }

            initialized = false;
        }

        void render() override {
            // TODO: mayby add an provider as input?
        }

        int write(const AudioBufferDeinterleaved<float> *buffer, int sample_count) override {
            int cursor = output.getSampleCount();
            output.setSampleCount(output.getSampleCount() + sample_count);

            for (int c = 0; c < output.getChannelCount(); ++c) {
                std::memcpy(output.getData(c) + sample_count, buffer->getChannel(c), sample_count * sizeof(float));
            }

            return sample_count;
        }
    };
}}

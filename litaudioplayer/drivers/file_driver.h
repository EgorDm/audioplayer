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
    template<typename T>
    class FileDriver : public DriverInterface<T> {
    protected:
        std::shared_ptr<AudioContainerInterface> output;
        AudioBufferDeinterleavedInterface<float> *source_buffer;

    public:
        FileDriver(const std::shared_ptr<playback::PlaybackInterface<T>> &playback) : DriverInterface<T>(playback) {}

        bool create(EngineProperties &properties) override {
            debug::log(FileDriver_TAG, "Creating FileDriver");

            output = std::make_shared<AudioContainerDeinterleaved<T>>(properties.channel_count, properties.sample_rate);

            this->initialized = true;
            return true;
        }

        void destroy() override {
            debug::log(FileDriver_TAG, "Destroying FileDriver");

            AudioWriter writer(&output, FILE_DRIVER_SAVE_PATH);
            if (!writer.write()) {
                debug::log_error(FileDriver_TAG, "FileDriver could not save the audio stream.");
            }

            this->initialized = false;
        }

        int render(int sample_count) override {
            int out_count = 0;
            this->getPlayback()->requestCurrent(this->buffer.get(), sample_count, out_count);
            this->getPlayback()->progress(out_count); // TODO write
            return out_count;
        }

        int write(const AudioBufferDeinterleaved<T> *buffer, int sample_count) override {
            int cursor = output->getSampleCount();
            output->setSampleCount(output->getSampleCount() + sample_count);

            for (int c = 0; c < output->getChannelCount(); ++c) {
                std::memcpy(source_buffer->getChannel(c) + sample_count, buffer->getChannel(c),
                            sample_count * buffer->getSampleSize());
            }

            return sample_count;
        }
    };
}}

//
// Created by egordm on 30-12-18.
//

#pragma once

#include <drivers/driver_interface.h>
#include <portaudio.h>
#include <utils/debug.h>
#include <utils/math.h>

using namespace litcore;
using namespace lit::math;

#define PortAudioDriver_TAG "PortAudioDriver"

namespace litaudioplayer { namespace drivers {
    class PortAudioDriver : public DriverInterface<float> {
    private:
        PaStream *stream;

    public:
        PortAudioDriver(playback::PlaybackInterface<float> *playback) : DriverInterface(playback) {}

        bool create(EngineProperties &properties) override {
            LIT_ASSERT(Pa_Initialize() == paNoError, PortAudioDriver_TAG, "Failed initializing portaudio", false);

            PaStreamParameters output_params;
            output_params.device = Pa_GetDefaultOutputDevice();
            LIT_ASSERT(output_params.device != paNoDevice, PortAudioDriver_TAG, "Cant find an output device", false);
            output_params.channelCount = properties.channel_count;
            output_params.sampleFormat = paInt16;
            output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)->defaultHighOutputLatency;
            output_params.hostApiSpecificStreamInfo = nullptr;

            PaError ret = Pa_OpenStream(&stream, nullptr, &output_params, properties.sample_rate,
                                        properties.buffer_size, 0, _stream_callback, this);
            if (ret != paNoError) {
                debug::log_error(PortAudioDriver_TAG, std::string("Pa_OpenStream failed: ") + Pa_GetErrorText(ret));
                if (stream) Pa_CloseStream(stream);
                return false;
            }

            return DriverInterface<float>::create(properties);
        }

        void destroy() override {

        }

        int render(int sample_count) override {
            int out_count;
            playback->render(buffer.get(), sample_count, out_count);
            playback->progress(out_count);
            return out_count;
        }

        void on_start() override {
            if (stream) Pa_StartStream(stream);
        }

        void on_pause() override {
            if (stream) Pa_StopStream(stream);
        }

        void on_stop() override {
            if (stream) Pa_StopStream(stream);
        }

    private:
        bool stream_callback(uint16_t *output, int frame_count) {
            int out_count = render(frame_count);

            // TODO: move into utils
            int max_cursor = out_count * buffer->getChannelCount();
            int channel_count = buffer->getChannelCount();
            int out_cursor, in_cursor;
            float *c_buffer;
            for (int c = 0; c < buffer->getChannelCount(); ++c) {
                c_buffer = buffer->getChannel(c);
                for (out_cursor = c, in_cursor = 0; out_cursor < max_cursor; out_cursor += channel_count) {
                    output[out_cursor] = (uint16_t) (clip(c_buffer[in_cursor++], -1, 1) * 32760);
                }
            }

            return out_count == 0;
        }

        static int _stream_callback(const void *input, void *output, unsigned long frameCount,
                                    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                    void *userData) {
            #if DEBUG > 3
            if(statusFlags & paOutputUnderflow) debug::log(PortAudioDriver_TAG, "Output underflow!");
            #endif

            auto driver = static_cast<PortAudioDriver *>(userData);
            bool completed = driver->stream_callback(reinterpret_cast<uint16_t *>(output), frameCount);
            return completed ? paComplete : paContinue; // TODO: notify change
        }
    };
}}

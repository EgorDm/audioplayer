//
// Created by egordm on 30-12-18.
//

#pragma once

#include <drivers/driver_interface.h>
#include <portaudio.h>
#include <utils/debug.h>
#include <utils/math.h>
#include <utils/conversion.h>

using namespace litcore;
using namespace lit::math;

#define PortAudioDriver_TAG "PortAudioDriver"

namespace litaudioplayer { namespace drivers {
    class PortAudioDriver : public DriverInterface<float> {
    private:
        PaStream *stream;
        PaError error = paNoError;

        PaStreamParameters getPaParams(EngineProperties &properties) {
            PaStreamParameters ret;
            ret.device = Pa_GetDefaultOutputDevice();
            ret.channelCount = properties.channel_count;
            ret.sampleFormat = paInt16;
            ret.suggestedLatency = Pa_GetDeviceInfo(ret.device)->defaultHighOutputLatency;
            ret.hostApiSpecificStreamInfo = nullptr;
            return ret;
        }

    public:
        PortAudioDriver(const std::shared_ptr<playback::PlaybackInterface<float>> &playback)
                : DriverInterface(playback) {}

        virtual ~PortAudioDriver() {
            Pa_Terminate();
        }

        bool create(EngineProperties &properties) override {
            // Initialize port audio
            LIT_ASSERT(error = Pa_Initialize() == paNoError, PortAudioDriver_TAG,
                       std::string("Failed initializing portaudio") + Pa_GetErrorText(error), false);

            PaStreamParameters pa_params = getPaParams(properties);
            LIT_ASSERT(pa_params.device != paNoDevice, PortAudioDriver_TAG, "Cant find an output device", false);

            error = Pa_OpenStream(&stream, nullptr, &pa_params, properties.sample_rate, properties.buffer_size, 0,
                                  _stream_callback, this);
            if (error != paNoError) {
                debug::log_error(PortAudioDriver_TAG, std::string("Pa_OpenStream failed: ") + Pa_GetErrorText(error));
                if (stream) Pa_CloseStream(stream);
                return false;
            }

            return DriverInterface<float>::create(properties);
        }

        void destroy() override {
            error = Pa_Terminate();
        }

        int render(int sample_count) override {
            int out_count = 0;
            playback->requestCurrent(buffer.get(), sample_count, out_count);
            playback->progress(out_count);
            return out_count;
        }

        void onStart() override {
            if (stream) error = Pa_StartStream(stream);
            LIT_ASSERT(error == paNoError, PortAudioDriver_TAG, std::string("OnStart Error:") + Pa_GetErrorText(error),);
        }

        void onPause() override {
            if (stream) error = Pa_StopStream(stream);
            LIT_ASSERT(error == paNoError, PortAudioDriver_TAG, std::string("OnPause Error:") + Pa_GetErrorText(error),);
        }

        void onStop() override {
            if (stream) error = Pa_StopStream(stream);
            LIT_ASSERT(error == paNoError, PortAudioDriver_TAG, std::string("OnStop Error:") + Pa_GetErrorText(error),);
        }

    private:
        bool stream_callback(uint16_t *output, int frame_count) {
            int out_count = render(frame_count);
            utils::convert_ifltp_dfixp(buffer.get(), output, out_count);
            return out_count == 0;
        }

        static int _stream_callback(const void *input, void *output, unsigned long frameCount,
                                    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                    void *userData) {
            #if DEBUG > 3
            // TODO: write to a var instead. No loggin in here!
            if (statusFlags & paOutputUnderflow) debug::log(PortAudioDriver_TAG, "Output underflow!");
            #endif

            auto driver = static_cast<PortAudioDriver *>(userData);
            bool completed = driver->stream_callback(reinterpret_cast<uint16_t *>(output), frameCount);
            return completed ? paComplete : paContinue; // TODO: notify change
        }
    };
}}

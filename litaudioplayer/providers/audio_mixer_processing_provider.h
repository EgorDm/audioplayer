//
// Created by egordm on 19-1-19.
//

#pragma once

#include <memory>
#include <algorithm>
#include <numeric>
#include <utils/audio_buffer_utils.h>
#include "audio_processing_provider.h"

namespace litaudioplayer { namespace providers {
    /*struct AudioMixerObserver {

    };*/

    template<typename T>
    class AudioMixerProcessingProvider : public AudioProcessingProvider<T> {
    protected:
        std::vector<std::shared_ptr<AudioProviderInterface<T>>> channels;
        std::vector<float> levels;
        int master_index = 0;

    public:
        AudioMixerProcessingProvider(int mix_count, int channel_count, int buffer_size)
                : AudioProcessingProvider<T>(nullptr) {
            channels.reserve(mix_count);
            levels.reserve(mix_count);
            for (int i = 0; i < mix_count; ++i) {
                channels.push_back(nullptr);
                levels.push_back(0.5f);
            }
        }

        void request(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int &out_sample_count, int cursor,
                     uint processing_flags) const override {
            if (channels.size() == 0) {
                out_sample_count = buffer->getSampleCount();
                return;
            }

            int tmp_out_sample_count = 0;
            for (int i = 0; i < channels.size(); ++i) {
                if (channels[i] == nullptr) continue;
                buffer->reset(); // TODO: just set signal for first. add for second ..
                channels[i]->request(swap, swap, sample_count, tmp_out_sample_count, cursor, processing_flags);
                litaudio::utils::mult_buffer(swap, 0, tmp_out_sample_count, levels[i]);
                litaudio::utils::add_buffers(buffer, swap, 0, 0, tmp_out_sample_count);
                out_sample_count = std::max(out_sample_count, tmp_out_sample_count);
            }
        }

        int getChannelCount() {
            return static_cast<int>(channels.size());
        }

        void setMaster(int index) {
            if (master_index == index) return;
            master_index = index;
            this->setChild(channels[master_index]);
        }

        std::shared_ptr<AudioProviderInterface<T>> &getMaster() {
            return this->getChild();
        }

        int getSampleCount() const override {
            return AudioProcessingProvider<T>::getSampleCount();
        }

        void setLevel(int index, float level) {
            levels[index] = level;
        }

        void setChannel(int index, const std::shared_ptr<AudioProviderInterface<T>> &channel) {
            channels[index] = channel;
            if(index == master_index) this->setChild(channel);
        }

        void process(AudioBufferDeinterleavedInterface<T> *buffer, AudioBufferDeinterleavedInterface<T> *swap,
                     int sample_count, int cursor) const override {
            // TODO: ignore
        }
    };
}}

//
// Created by egordm on 18-1-19.
//

#pragma once

#include <memory>
#include "playback_queue.h"
#include "simple_playback.h"

namespace litaudioplayer { namespace playback {
    template<typename T, typename I>
    struct AudioProviderLoader {
        virtual std::shared_ptr<providers::AudioProviderInterface<T>> load(I item) = 0;
    };

    template<typename T, typename I>
    class QueuedPlayback : public SimplePlayback<T>, public PlaybackQueue<I>::Listener {
    protected:
        PlaybackQueue<I> queue;
        AudioProviderLoader<T, I *> *loader;

    public:
        using Loader = AudioProviderLoader<T, I *>;
        using QueueObserver = PlaybackQueueListener<I*>;

        explicit QueuedPlayback(AudioProviderLoader<T, I *> *loader,
                                const std::shared_ptr<providers::AudioProviderInterface<T>> &provider = nullptr)
                : SimplePlayback<T>(provider), loader(loader) {
            queue.addObserver(this);
        }

        PlaybackQueue<I> &getQueue() {
            return queue;
        }

        void onCurrentChange(I *current) override {
            this->setProvider(loader->load(current));
        }
    };
}}

//
// Created by egordm on 18-1-19.
//

#pragma once

#include <providers/audio_provider_interface.h>
#include "playback_queue_interface.h"

namespace litaudioplayer { namespace playback {
    template<typename T>
    class PlaybackQueue : public PlaybackQueueInterface<T*> {
    protected:
        std::vector<T*> queue;
        T* current = nullptr;
        int cursor = 0;

    public:
        using Listener = PlaybackQueueListener<T*>;

        const std::vector<T *> &getItems() const {
            return queue;
        }

        void next() override {
            if(queue.empty()) return;
            if(cursor + 1 < queue.size()) setCurrent(cursor + 1);
        }

        void previous() override {
            if (queue.empty()) return;
            if(cursor - 1 >= 0) setCurrent(cursor - 1);
        }

        void setCurrent(int index) override {
            if(index >= queue.size()) return;
            cursor = index;
            current = queue[index];
            EACH_OBSERVER(observer->onCurrentChange(current));
        }

        T *getCurrent() override {
            return current;
        }

        void enqueue(T *item) override {
            queue.push_back(item);
            EACH_OBSERVER(observer->onEnqueued(item));
        }

        void enqueue(T* item, int index) override {
            if(index <= cursor) cursor += cursor;
            queue.insert(queue.begin() + index, item);
            EACH_OBSERVER(observer->onEnqueued(item));
        }

        void reorder(int index, int to) override {
            if(index > cursor && to <= cursor) cursor += 1;
            if(index < cursor && to > cursor) cursor -= 1;
            if(index == cursor) cursor = to;
            auto swap = queue[index];
            queue.erase(queue.begin() + index);
            queue.insert(queue.begin() + to, swap);
            EACH_OBSERVER(observer->onQueueChanged());
        }

        T *dequeue(int index) override {
            if(index <= cursor) cursor -= 1;
            auto ret = queue[index];
            queue.erase(queue.begin() + index);
            EACH_OBSERVER(observer->onDequeued(ret));
            return ret;
        }

        void setShuffle(bool shuffle) override {

        }

        void setRepeat(bool repeat) override {

        }
    };
}}

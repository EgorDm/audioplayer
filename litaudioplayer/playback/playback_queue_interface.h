//
// Created by egordm on 18-1-19.
//

#pragma once

#include <memory>
#include <vector>
#include <helpers/observable.h>

namespace litaudioplayer { namespace playback {
    template<typename T>
    class PlaybackQueueListener {
    public:
        virtual void onCurrentChange(T current) {}

        virtual void onEnqueued(T item) {}

        virtual void onDequeued(T item) {}

        virtual void onQueueChanged() {}
    };

    template<typename T>
    class PlaybackQueueInterface : public helpers::Observable<PlaybackQueueListener<T>> {
    public:
        virtual void next() = 0;

        virtual void previous() = 0;

        virtual void setCurrent(int index) = 0;

        virtual T getCurrent() = 0;

        virtual void enqueue(T item) = 0;

        virtual void enqueue(T item, int index) = 0;

        virtual T dequeue(int index) = 0;

        virtual void reorder(int index, int to) = 0;

        virtual void setShuffle(bool shuffle) = 0;

        virtual void setRepeat(bool repeat) = 0;
    };
}}

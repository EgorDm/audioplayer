//
// Created by egordm on 18-1-19.
//

#pragma once

#include <unordered_set>
#include <memory>
#include <algorithm>

#define EACH_OBSERVER(a) {for(const auto &observer : this->observers) a;}

namespace litaudioplayer { namespace helpers {
    template<typename T>
    class Observable {
    protected:
        std::unordered_set<T*> observers;

    public:
        void addObserver(T* observer) {
            if(observer == nullptr) return;
            observers.insert(observer);
        }

        void removeObserver(T* observer) {
            observers.erase(observer);
        }
    };
}}

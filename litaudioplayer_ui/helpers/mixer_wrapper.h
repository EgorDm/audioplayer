//
// Created by egordm on 29-1-19.
//

#pragma once

#include <QObject>
#include <mixer_widget.h>
#include <providers/audio_mixer_processing_provider.h>

namespace litaudioplayer { namespace helpers {
    class MixerWrapper : public QObject {
    protected:
        litwidgets::MixerWidget *widget;
        std::shared_ptr<providers::AudioMixerProcessingProvider<float>> mixer_provider;

    public:
        MixerWrapper(litwidgets::MixerWidget *widget,
                     const std::shared_ptr<providers::AudioMixerProcessingProvider<float>> &mixer_provider);

        void on_mixerWidget_onChannelChanged(int index);

        void on_mixerWidget_onChannelValueChanged(int index, int value);

        void addChannel(const std::string &name, int channel);
    };
}};



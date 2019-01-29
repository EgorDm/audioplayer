//
// Created by egordm on 29-1-19.
//

#include <utils/audio_utils.h>
#include "mixer_wrapper.h"

using namespace litaudioplayer::helpers;

MixerWrapper::MixerWrapper(litwidgets::MixerWidget *widget,
                           const std::shared_ptr<providers::AudioMixerProcessingProvider<float>> &mixer_provider)
        : widget(widget), mixer_provider(mixer_provider) {
    connect(widget, &litwidgets::MixerWidget::onChannelChanged, this, &MixerWrapper::on_mixerWidget_onChannelChanged);


}

void MixerWrapper::on_mixerWidget_onChannelChanged(int index) {
    int value = widget->getChannelDb(index);
    mixer_provider->setLevel(index, litaudio::audio_utils::dbToAmplitude(value));
}

void MixerWrapper::addChannel(const std::string &name, int channel) {
    widget->addMix(name, static_cast<int>(litaudio::audio_utils::amplitudeToDb(mixer_provider->getLevel(channel))));

}

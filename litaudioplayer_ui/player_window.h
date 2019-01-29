//
// Created by egordm on 1-1-19.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <audio_engine.h>
#include <drivers/portaudio_driver.h>
#include "playback/litaudioplayback.h"
#include <QtWidgets/QListWidgetItem>
#include <QTimer>
#include <unordered_map>
#include <helpers/mixer_wrapper.h>
#include "audio_item.h"
#include "litaudioplayer.h"

namespace Ui {
    class PlayerWindow;
}

using namespace litaudioplayer;
using namespace litaudioplayer::structs;

class PlayerWindow
        : public QMainWindow,
          public playback::PlaybackControllerObserver,
          public playback::LitAudioPlayback::QueueObserver,
          public playback::LitAudioPlayback::ObserverType {
Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);

    ~PlayerWindow() override;

    void onStart() override;

    void onPause() override;

    void onStop() override;

    void onCurrentChange(AudioItemDescriptor *current) override;

    void onEnqueued(AudioItemDescriptor *item) override;

    void onDequeued(AudioItemDescriptor *item) override;

    void onQueueChanged() override;

    void changeFocus(bool recalculate);

private:
    void onProviderChange(const std::shared_ptr<AudioProviderInterface<float>> &provider) override;

private slots:

    void on_add_clicked();

    void on_remove_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_play_clicked();

    void on_seekbar_cursorChangedEvent(float cursor);

    void on_volumeBar_sliderMoved(int value);

    void on_shuffle_clicked();

    void on_metronomeWidget_startClicked(bool down);

    void on_metronomeWidget_detectClicked();

    void update();

private:
    int getItemIndex(int uid);

private:
    Ui::PlayerWindow *ui;

    std::unique_ptr<helpers::MixerWrapper> mixer_wrapper;

    QTimer *updater = new QTimer(this);

    LitAudioplayer player;
};


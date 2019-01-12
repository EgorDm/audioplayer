//
// Created by egordm on 1-1-19.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <audio_engine.h>
#include <drivers/portaudio_driver.h>
#include <playback/simple_playback.h>
#include <QtWidgets/QListWidgetItem>
#include <QTimer>
#include <unordered_map>
#include "audio_item.h"

namespace Ui {
    class PlayerWindow;
}

using namespace litaudioplayer;

class PlayerWindow : public QMainWindow, public playback::PlaybackListener {
Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);

    ~PlayerWindow() override;

    void onStart() override;

    void onPause() override;

    void onStop() override;

    void startSong(QListWidgetItem *item);

private slots:
    void on_add_clicked();

    void on_remove_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_play_clicked();

    void on_seekbar_cursorChangedEvent(float cursor);

    void on_volumeBar_sliderMoved(int value);

    void on_shuffle_clicked();

    void update();

private:
    void run_test();

    Ui::PlayerWindow *ui;

    QTimer *updater = new QTimer(this);

    std::unique_ptr<AudioEngine<float>> engine;
    std::shared_ptr<playback::SimplePlayback<float>> playback;

    std::unordered_map<std::string, AudioItem> item_data;

    QListWidgetItem *current_item = nullptr;
};


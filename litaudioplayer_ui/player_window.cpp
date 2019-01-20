//
// Created by egordm on 1-1-19.
//

#include <providers/audio_source_provider.h>
#include <audiofile/reading.h>
#include <QtWidgets/QFileDialog>
#include <algorithm_structure/frame_factories/frame_factory_vec_provider.h>
#include <providers/audio_metronome_source_provider.h>
#include "player_window.h"
#include "ui_player_window.h"

using namespace litaudiofile;

PlayerWindow::PlayerWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::PlayerWindow) {
    ui->setupUi(this);
    //this->setFixedSize(this->geometry().width(), this->geometry().height());

    connect(updater, SIGNAL(timeout()), this, SLOT(update()));

    ui->volumeBar->setValue(ACI(player.getPlayback()->getVolumeProcessor()->getVolumeDb()));
    player.getQueue().addObserver(this);
    player.getPlayback()->addObserver(this);
    player.getEngine()->getController()->addObserver(this);
}

PlayerWindow::~PlayerWindow() {
    delete ui;
}

void PlayerWindow::on_add_clicked() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open Audio file"), "data",
                                                tr("Audio file (*.mp3 *.wav *.flac);;All Files (*)"));
    if (path.isNull()) return;
    player.addAudioFile(path.toStdString());
}

void PlayerWindow::on_remove_clicked() {
    for (auto item : ui->listWidget->selectedItems()) {
        int item_index = getItemIndex(item->data(Qt::UserRole).toInt());
        if (item_index >= 0) player.getQueue().dequeue(item_index);
    }
}

void PlayerWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    int item_index = getItemIndex(item->data(Qt::UserRole).toInt());
    if (item_index < 0) return;

    player.getEngine()->getController()->stop();
    player.getQueue().setCurrent(item_index);
    player.getEngine()->getController()->start();
}

void PlayerWindow::on_play_clicked() {
    if (ui->play->isChecked()) player.getEngine()->getController()->start();
    else player.getEngine()->getController()->stop();
}

void PlayerWindow::onStart() {
    ui->play->setChecked(true);
    updater->start();
}

void PlayerWindow::onPause() {
    ui->play->setChecked(false);
    updater->stop();
}

void PlayerWindow::onStop() {
    ui->play->setChecked(false);
    updater->stop();
}

void PlayerWindow::update() {
    if (!ui->seekbar->isMovingCursor()) {
        float position = player.getEngine()->getPlayback()->getCursor() /
                         (float) player.getEngine()->getPlayback()->getSampleCount();
        ui->seekbar->setCursor(position);
        ui->seekbar->repaint();
    }
}

void PlayerWindow::on_seekbar_cursorChangedEvent(float cursor) {
    player.getEngine()->getController()->seek(cursor);
}

void PlayerWindow::on_volumeBar_sliderMoved(int value) {
    player.getPlayback()->getVolumeProcessor()->setVolumeDb(value / (float) 1);
}

void PlayerWindow::on_shuffle_clicked() {
    // run_test();
}

int PlayerWindow::getItemIndex(int uid) {
    int item_index = -1;
    for (int i = 0; i < player.getQueue().getItems().size(); ++i) {
        if (uid == player.getQueue().getItems()[i]->uid) item_index = i;
    }
    return item_index;
}

void PlayerWindow::onCurrentChange(AudioItemDescriptor *current) {
    ui->currentSong->setText(QString::fromStdString(current->title));
}

void PlayerWindow::onEnqueued(AudioItemDescriptor *item) {
    auto listItem = new QListWidgetItem(QString::fromStdString(item->title));
    listItem->setData(Qt::UserRole, item->uid);
    ui->listWidget->addItem(listItem);
}

void PlayerWindow::onDequeued(AudioItemDescriptor *item) {
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if (ui->listWidget->item(i)->data(Qt::UserRole).toInt() != item->uid) continue;
        auto listItem = ui->listWidget->item(i);
        ui->listWidget->removeItemWidget(listItem);
        delete listItem;
    }
}

void PlayerWindow::onQueueChanged() {
    PlaybackQueueListener::onQueueChanged();
}

void PlayerWindow::onProviderChange(const std::shared_ptr<AudioProviderInterface<float>> &provider) {
    // TODO: copy shptr
    ui->seekbar->setInput(new algorithm::FrameFactoryVecProvider<float>(player.getPlayback().get(), 1, 1));
}

void PlayerWindow::on_metronomeWidget_startClicked(bool down) {
    auto time_signature = ui->metronomeWidget->getTimeSignature();
    player.getPlayback()->setTimeSignature(time_signature);
    player.getPlayback()->getMixerProvider()->setMaster(1);

    std::stringstream ss;
    ss << "Metronome - BPM: " << time_signature.getBpm() << ", TS: " << time_signature.getUpper() << "/" << time_signature.getLower();
    auto item = new struct::AudioItemDescriptor(ss.str(), false, "", true, time_signature);
    player.getEngine()->getController()->stop();
    player.getQueue().setCurrent(item);
    player.getEngine()->getController()->start();

    ui->seekbar->setInput(new algorithm::FrameFactoryVecProvider<float>(player.getPlayback().get(), 1, 1));
}

void PlayerWindow::on_metronomeWidget_detectClicked() {

}



//
// Created by egordm on 1-1-19.
//

#include <providers/audio_source_provider.h>
#include <audiofile/reading.h>
#include <QtWidgets/QFileDialog>
#include "player_window.h"
#include "ui_player_window.h"

using namespace litaudiofile;

PlayerWindow::PlayerWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::PlayerWindow) {
    ui->setupUi(this);
    this->setFixedSize(this->geometry().width(), this->geometry().height());

    EngineProperties properties(2, 44100, 2048);
    playback = std::make_shared<playback::SimplePlayback<float>>();
    engine = std::unique_ptr<AudioEngine<float>>(create_engine<float, drivers::PortAudioDriver>(properties, playback));
    engine->getController()->addListener(this);

    connect(updater, SIGNAL(timeout()), this, SLOT(update()));
}

PlayerWindow::~PlayerWindow() {
    delete ui;
}

void PlayerWindow::on_add_clicked() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open Audio file"), "data",
                                                tr("Audio file (*.mp3 *.wav *.flac);;All Files (*)"));
    if(path.isNull()) return;

    auto data = create_audio_item(path.toStdString());
    item_data[path.toStdString()] = data;
    auto item = new QListWidgetItem(QString::fromStdString(data.title));
    item->setData(Qt::UserRole, path);
    ui->listWidget->addItem(item);
}

void PlayerWindow::on_remove_clicked() {
    if(ui->listWidget->selectedItems().contains(current_item)) {
        engine->getController()->stop();
        current_item = nullptr;
        ui->currentSong->setText("Song");
    }

    for (auto item : ui->listWidget->selectedItems()) {
        item_data.erase(item->data(Qt::UserRole).toString().toStdString());
    }

    qDeleteAll(ui->listWidget->selectedItems());
}

void PlayerWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    if(item != current_item) startSong(item);
}

void PlayerWindow::on_play_clicked() {
    if (!ui->play->isChecked() && current_item != nullptr) engine->getController()->start();
    else engine->getController()->stop();
}

void PlayerWindow::onStart() {
    ui->play->setChecked(false);
    if(current_item != nullptr) {
        auto data = item_data.at(current_item->data(Qt::UserRole).toString().toStdString());
        ui->currentSong->setText(QString::fromStdString(data.title));
        updater->start();
    }
}

void PlayerWindow::onPause() {
    ui->play->setChecked(true);
    updater->stop();
}

void PlayerWindow::onStop() {
    ui->play->setChecked(true);
    updater->stop();
}

void PlayerWindow::startSong(QListWidgetItem *item) {
    std::string path = item->data(Qt::UserRole).toString().toStdString();
    auto src = std::make_shared<AudioContainer<float>>(AV_SAMPLE_FMT_FLTP);
    AudioReader reader(src.get(), path);
    if(!reader.read()) return;

    engine->getController()->stop();
    playback->setProvider(std::make_shared<providers::AudioSourceProvider<float>>(src));
    current_item = item;
    engine->getController()->start();
}

void PlayerWindow::update() {
    if(!ui->seekBar->isSliderDown()) {
        ui->seekBar->setValue(
                static_cast<int>(engine->getPlayback()->getCursor() / (double)engine->getPlayback()->getSampleCount() * 1000));
    }
}

void PlayerWindow::on_seekBar_sliderMoved(int position) {
    int cursor = static_cast<int>(position / 1000. * engine->getPlayback()->getSampleCount());
    engine->getPlayback()->setCursor(cursor);
}

//
// Created by egordm on 19-1-19.
//

#include "metronome_widget.h"
#include "ui_metronome_widget.h"
#include "../litaudioplayer/structs/time_signature.h"

using namespace litwidgets;

MetronomeWidget::MetronomeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MetronomeWidget) {
    ui->setupUi(this);
}

const structs::TimeSignature &MetronomeWidget::getTimeSignature() const {
    return time_signature;
}

void MetronomeWidget::setTimeSignature(const structs::TimeSignature &time_signature) {
    MetronomeWidget::time_signature = time_signature;
    ui->spbMetronomeBPM->setValue(static_cast<int>(time_signature.getBpm()));
    ui->spbMetronomeNumerator->setValue(time_signature.getUpper());
    ui->spbMetronomeDenominator->setValue(time_signature.getLower());
    ui->spbOffset->setValue(time_signature.getOffset());
}

void MetronomeWidget::on_spbMetronomeBPM_valueChanged(int bpm) {
    time_signature.setBpm(bpm);
}

void MetronomeWidget::on_spbMetronomeNumerator_valueChanged(int value) {
    time_signature.setUpper(value);
}

void MetronomeWidget::on_spbMetronomeDenominator_valueChanged(int value) {
    time_signature.setLower(value);
}

void MetronomeWidget::on_spbOffset_valueChanged(double value) {
    time_signature.setOffset(value);
}

bool MetronomeWidget::getStartWithAudio() const {
    return ui->chkMetronomeWithAudio->isChecked();
}

void MetronomeWidget::setStartWithAudio(bool value) {
    return ui->chkMetronomeWithAudio->setChecked(value);
}

void MetronomeWidget::on_btnMetronomeStart_clicked() {
    emit startClicked(ui->btnMetronomeStart->isChecked());
}

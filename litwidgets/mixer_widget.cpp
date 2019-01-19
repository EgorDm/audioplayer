//
// Created by egordm on 19-1-19.
//

#include "mixer_widget.h"
#include "ui_mixer_widget.h"

using namespace litwidgets;

MixerWidget::MixerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MixerWidget) {
    ui->setupUi(this);
}
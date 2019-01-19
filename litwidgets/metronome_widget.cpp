//
// Created by egordm on 19-1-19.
//

#include "metronome_widget.h"
#include "ui_metronome_widget.h"

using namespace litwidgets;

MetronomeWidget::MetronomeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MetronomeWidget) {
    ui->setupUi(this);
}

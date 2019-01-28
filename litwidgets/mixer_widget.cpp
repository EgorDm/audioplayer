//
// Created by egordm on 19-1-19.
//

#include <QtWidgets/QSlider>
#include "mixer_widget.h"
#include "ui_mixer_widget.h"

using namespace litwidgets;

MixerWidget::MixerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MixerWidget) {
    ui->setupUi(this);
}

int MixerWidget::addMix(const std::string &name, int value) {
    int index = channels.size();
    auto stub = createMixStub();
    stub->findChild<QLabel *>()->setText(QString::fromStdString(name));
    auto slider = stub->findChild<QSlider *>();
    slider->setMaximum(150);
    slider->setValue(value);

    connect(slider, &QSlider::sliderReleased, [=]() {
        emit onChannelChanged(index);
    });

    channels.push_back(stub);
    return index;
}

int MixerWidget::getChannelValue(int index) {
    return channels[index]->findChild<QSlider *>()->value();
}

void MixerWidget::setChannelValue(int index, int value) {
    channels[index]->findChild<QSlider *>()->setValue(value);
}

QWidget *MixerWidget::createMixStub() {
    auto *widget = new QWidget(this);
    auto *layout = new QVBoxLayout();
    widget->setLayout(layout);

    layout->addWidget(new QLabel(), 0, Qt::AlignHCenter);
    layout->addWidget(new QSlider(), 0, Qt::AlignHCenter);
    ui->mixerContainerLayout->addWidget(widget, 0, Qt::AlignHCenter);

    return widget;
}

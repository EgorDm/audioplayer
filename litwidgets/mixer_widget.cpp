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
    int index = static_cast<int>(channels.size());
    auto stub = createMixStub();
    stub->findChild<QLabel *>()->setText(QString::fromStdString(name));
    auto slider = stub->findChild<QSlider *>();
    slider->setMaximum(maximum);
    slider->setMinimum(minimum);
    slider->setValue(value);

    connect(slider, &QSlider::sliderReleased, [=]() {
        emit onChannelChanged(index);
    });

    channels.push_back(stub);
    return index;
}

int MixerWidget::getChannelDb(int index) {
    return channels[index]->findChild<QSlider *>()->value();
}

void MixerWidget::setChannelDb(int index, int value) {
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

int MixerWidget::getMinimum() const {
    return minimum;
}

void MixerWidget::setMinimum(int minimum) {
    MixerWidget::minimum = minimum;
    for(auto widget : channels) widget->findChild<QSlider *>()->setMinimum(minimum);
}

int MixerWidget::getMaximum() const {
    return maximum;
}

void MixerWidget::setMaximum(int maximum) {
    MixerWidget::maximum = maximum;
    for(auto widget : channels) widget->findChild<QSlider *>()->setMaximum(maximum);
}

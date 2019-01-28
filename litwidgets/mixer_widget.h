//
// Created by egordm on 19-1-19.
//

#pragma once

#include <QtWidgets/QWidget>

namespace Ui {
    class MixerWidget;
}

namespace litwidgets {
    class MixerWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MixerWidget(QWidget *parent = nullptr);

        int addMix(const std::string &name, int value = 100);

        int getChannelValue(int index);

        void setChannelValue(int index, int value);

    signals:
        void onChannelChanged(int index);

    protected:
        QWidget *createMixStub();

    private:
        Ui::MixerWidget *ui;

        std::vector<QWidget *> channels;
    };
}




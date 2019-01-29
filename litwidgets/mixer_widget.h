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

        int getChannelDb(int index);

        void setChannelDb(int index, int value);

        int getMinimum() const;

        void setMinimum(int minimum);

        int getMaximum() const;

        void setMaximum(int maximum);

    signals:
        void onChannelChanged(int index);

    protected:
        QWidget *createMixStub();

        int minimum = -46;
        int maximum = 46;

    private:
        Ui::MixerWidget *ui;

        std::vector<QWidget *> channels;
    };
}




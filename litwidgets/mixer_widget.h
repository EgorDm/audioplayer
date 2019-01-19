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

    private:
        Ui::MixerWidget *ui;
    };
}




//
// Created by egordm on 19-1-19.
//

#pragma once


#include <QtWidgets/QWidget>

namespace Ui {
    class MetronomeWidget;
}

namespace litwidgets {
    class MetronomeWidget : public QWidget {
    Q_OBJECT
    public:
        explicit MetronomeWidget(QWidget *parent = nullptr);

    private:
        Ui::MetronomeWidget *ui;
    };
}



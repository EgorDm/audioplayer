//
// Created by egordm on 2-1-19.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <waveform_widget.h>

namespace Ui {
    class TestWindow;
}

class TestWindow : public QMainWindow {
Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = nullptr);

private:
    Ui::TestWindow *ui;
};



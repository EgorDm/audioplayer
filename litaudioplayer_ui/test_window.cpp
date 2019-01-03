//
// Created by egordm on 2-1-19.
//

#include <iostream>
#include "test_window.h"
#include "ui_test_window.h"

TestWindow::TestWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TestWindow) {
    ui->setupUi(this);
    //this->setFixedSize(this->geometry().width(), this->geometry().height());

    qDebug() << palette().color(backgroundRole());
}

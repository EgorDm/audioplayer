//
// Created by egordm on 29-12-18.
//

#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    std::cout << "Hello, World!" << std::endl;

    return application.exec();
}
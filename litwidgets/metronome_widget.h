//
// Created by egordm on 19-1-19.
//

#pragma once

#include <QtWidgets/QWidget>
#include <structs/time_signature.h>

using namespace litaudioplayer;

namespace Ui {
    class MetronomeWidget;
}

namespace litwidgets {
    class MetronomeWidget : public QWidget {
    Q_OBJECT
    protected:
        structs::TimeSignature time_signature;

    public:
        explicit MetronomeWidget(QWidget *parent = nullptr);

        const structs::TimeSignature &getTimeSignature() const;

        void setTimeSignature(const structs::TimeSignature &time_signature);

        bool getStartWithAudio() const;

        void setStartWithAudio(bool value);

    private slots:
        void on_spbMetronomeBPM_valueChanged(int value);

        void on_spbMetronomeNumerator_valueChanged(int value);

        void on_spbMetronomeDenominator_valueChanged(int value);

        void on_spbOffset_valueChanged(double value);

        void on_btnMetronomeStart_clicked();

    signals:
        void startClicked(bool down);

        void detectClicked();

    private:
        Ui::MetronomeWidget *ui;


    };
}



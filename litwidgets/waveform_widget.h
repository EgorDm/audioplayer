//
// Created by egordm on 2-1-19.
//

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QtGui/QOpenGLTexture>
#include <algorithm_structure/frame_factory_vec.h>
#include <algorithm_structure/output_builder_mat.h>
#include <algorithm_structure/algorithm_simple_runner.h>
#include "opengl_helper.h"
#include "waveform_generator.h"

using namespace litsignal::algorithm;

namespace litwidgets {
    class WaveformWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    public:
        WaveformWidget(QWidget *parent = nullptr);

        void changeInput(const fvec &input, bool redraw = true);

    protected:
        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    private slots:
        void updateResize();

    protected:
        QOpenGLShaderProgram *program;
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;

        Mesh<Vertex2D> quad;
        QOpenGLTexture *waveData;
        std::vector<float> test;

        AlgorithmSimpleRunner<WaveformPipeline> runner;
        WaveformGenerator generator;
        FrameFactoryVec<float> *frameFactory;
        OutputBuilderMat<float> *outputBuilder;
        WaveformPipeline *pipeline;
        fvec input;

        int current_size = -1;

        QTimer *timer;
    };
}

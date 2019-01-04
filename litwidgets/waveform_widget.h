//
// Created by egordm on 2-1-19.
//

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QTimer>
#include <QOpenGLTexture>
#include <algorithm_structure/frame_factory_vec.h>
#include <algorithm_structure/output_builder_row_mat.h>
#include <algorithm_structure/algorithm_simple_runner.h>
#include "opengl_helper.h"
#include "waveform_generator.h"

using namespace litsignal::algorithm;

namespace litwidgets {
    class WaveformWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
    public:
        explicit WaveformWidget(QWidget *parent = nullptr);

    protected:
        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

        virtual QString getFragmentShader();

        virtual void initDefaultUniform();

    private slots:
        void updateResize();

    private:
        void updateWaveformTexture();

    protected:
        const QString VERTEX_SHADER = ":shaders/default_plane.vs.glsl";

        QOpenGLShaderProgram *program;
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;

        Mesh<Vertex2D> quad;
        QOpenGLTexture *waveform;

        WaveformGenerator generator;
        fvec input;

        int current_size = -1;

        QTimer timer;
    };
}

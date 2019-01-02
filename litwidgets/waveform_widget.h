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
#include "opengl_helper.h"

namespace litwidgets {


    class WaveformWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    public:
        WaveformWidget(QWidget *parent = nullptr);

    protected:
        void initializeGL() override;

        void resizeGL(int w, int h) override;

        void paintGL() override;

    protected:
        QOpenGLShaderProgram *program;
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;

        Mesh<Vertex2D> quad;
        QOpenGLTexture *waveData;
        std::vector<float> test;
    };
}

//
// Created by egordm on 2-1-19.
//

#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLPixelTransferOptions>
#include "waveform_widget.h"
#include "opengl_helper.h"
#include <cmath>

using namespace litwidgets;

WaveformWidget::WaveformWidget(QWidget *parent)
        : QOpenGLWidget(parent), quad(create_quad()) {}

void WaveformWidget::initializeGL() {
    initializeOpenGLFunctions();

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/default_plane.vs.glsl");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/waveform.fs.glsl");
    program->bindAttributeLocation("inPos", 0);
    program->bindAttributeLocation("inUV", 1);
    program->link();

    program->bind();
    program->setUniformValue("uTexture", 0);
    program->setUniformValue("uBackgroundColor", QColor(55, 62, 64));
    program->setUniformValue("uPrimaryColor", QColor(128, 147, 241));
    program->setUniformValue("uSecondaryColor", QColor(117, 185, 214));

    vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(quad.getVertexData(), quad.getVertexDataSize());

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    int width = 300;
    test.resize(width * 2);
    for (int i = 0; i < width; ++i) {
        test[i] = abs(rand() / (float)RAND_MAX);
        test[width + i] = test[i] * powf(test[i], 2) * abs(rand() / (float)RAND_MAX);
    }

    waveData = new QOpenGLTexture(QOpenGLTexture::Target2D);
    waveData->setSize(width, 2);
    waveData->setFormat(QOpenGLTexture::R32F);
    waveData->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    waveData->allocateStorage();
    waveData->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, test.data());
}

void WaveformWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
}

void WaveformWidget::paintGL() {
    glClearColor(1.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    program->bind();
    waveData->bind(0);

    glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
}

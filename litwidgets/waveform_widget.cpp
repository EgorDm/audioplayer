//
// Created by egordm on 2-1-19.
//

#include <iostream>
#include <QOpenGLFunctions>
#include "waveform_widget.h"
#include "opengl_helper.h"

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
}

void WaveformWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
}

void WaveformWidget::paintGL() {
    glClearColor(1.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    program->bind();
    glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
}

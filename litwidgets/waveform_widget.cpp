//
// Created by egordm on 2-1-19.
//

#include <iostream>
#include <QOpenGLFunctions>
#include "waveform_widget.h"
#include "opengl_helper.h"

using namespace litwidgets;

static const char *vshaderSource =
        "#version 150\n"
        "in vec3 inPos;\n"
        "in vec2 inUV;\n"
        "out vec2 UV;\n"
        "void main() {\n"
        "   UV = inUV;\n"
        "   gl_Position = vec4(inPos, 1.0);\n"
        "}\n";

static const char *fshaderSource =
        "#version 150\n"
        "out vec4 outColor;\n"
        "in vec2 UV;\n"
        "void main() {\n"
        "   outColor = vec4(UV.x, 0, UV.y, 0);\n"
        "}\n";


WaveformWidget::WaveformWidget(QWidget *parent)
        : QOpenGLWidget(parent), quad(create_quad()) {}

void WaveformWidget::initializeGL() {
    initializeOpenGLFunctions();
    std::cout << sizeof(Vertex2D) << std::endl;

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshaderSource);
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

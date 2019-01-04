//
// Created by egordm on 2-1-19.
//

#include <iostream>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QOpenGLPixelTransferOptions>
#include "waveform_widget.h"
#include "opengl_helper.h"
#include <audiofile/reading.h>
#include <cmath>
#include <structures/signal_container.h>

using namespace litwidgets;
using namespace litsignal;
using namespace litaudiofile;

WaveformWidget::WaveformWidget(QWidget *parent)
        : QOpenGLWidget(parent), quad(create_quad()) {
    frameFactory = new FrameFactoryVec<float>(input, 100, 100);
    outputBuilder = new OutputBuilderMat<float>(2);
    pipeline = new WaveformPipeline(frameFactory, outputBuilder, generator);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateResize()));
}

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
    program->setUniformValue("uBackgroundColor", palette().color(QPalette::Background));
    program->setUniformValue("uPrimaryColor", palette().color(QPalette::Highlight));
    program->setUniformValue("uSecondaryColor", palette().color(QPalette::Light));
    program->setUniformValue("uFocusBorderColor", QColor(255, 209, 102));
    program->setUniformValue("uFocusWindow", QVector2D(0.2f, 0.8f));
    program->setUniformValue("uMarkerBorderColor", QColor(239, 111, 108));
    program->setUniformValue("uCursorBorderColor", QColor(0, 148, 198));
    program->setUniformValue("uCursor", 0.1f);
    for (int i = 0; i < 10; ++i) {
        std::string name = "uMarkers[" + std::to_string(i) + "]";
        program->setUniformValue(name.c_str(), abs(rand() / (float)RAND_MAX));
    }

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

    waveData = new QOpenGLTexture(QOpenGLTexture::Target2D);
    waveData->setFormat(QOpenGLTexture::R32F);
    waveData->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);

    litsignal::structures::SignalContainer src;
    AudioReader reader(&src, "data/hangar.mp3");
    reader.read();
    fvec test = conv_to<fvec>::from(src.get_data_vec().col(0));
    changeInput(test, false);
}

void WaveformWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    timer->stop();
    timer->start(1000);
}

void WaveformWidget::paintGL() {
    glClearColor(1.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    program->bind();
    waveData->bind(0);

    glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
}

void WaveformWidget::changeInput(const fvec &input, bool redraw) {
    this->input = input;

    int frame_size = (int) std::ceil(input.size() / (float) width());
    //frameFactory->setInput(WaveformWidget::input);
    frameFactory->setFrameSize(frame_size);
    frameFactory->setHopSize(frame_size);

    outputBuilder->reset();
    outputBuilder->resize(frameFactory->getFrameCount());

    if (frameFactory->getFrameCount() > 1) {
        runner.run(pipeline);
    }

    fmat test = outputBuilder->getOutput().t();
    test(span::all, 0) /= test(span::all, 0).max();

    if (test.n_rows != current_size) {
        if(current_size != -1) {
            waveData->destroy();
            waveData->setFormat(QOpenGLTexture::R32F);
            waveData->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        }
        current_size = test.n_rows;
        waveData->setSize(current_size, 2);
        waveData->allocateStorage();
    }

    waveData->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, test.memptr());

    if (redraw) repaint();
}

void WaveformWidget::updateResize() {
    std::cout << "New Size: " << width() << std::endl;
    changeInput(input, true);
}

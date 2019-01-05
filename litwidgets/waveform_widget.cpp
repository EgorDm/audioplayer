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
        : QOpenGLWidget(parent), quad(create_quad()), generator(input, width()), timer(this) {
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateResize()));

    litsignal::structures::SignalContainer src;
    AudioReader reader(&src, "data/hangar.mp3");
    reader.read();
    input = conv_to<fvec>::from(src.get_data_vec().col(0));

    generator.setInput(input);
    generator.setWidth(width());
    generator.recalculate();
}

void WaveformWidget::initializeGL() {
    initializeOpenGLFunctions();

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, getFragmentShader());
    program->bindAttributeLocation("inPos", 0);
    program->bindAttributeLocation("inUV", 1);
    program->link();
    program->bind();
    initDefaultUniform();

    // Create the quad we will render to
    vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);

    vbo.create();
    vbo.bind();
    vbo.allocate(quad.getVertexData(), quad.getVertexDataSize());
    Vertex2D::initVBOLayout();

    // Fill waveform texture
    waveform = new QOpenGLTexture(QOpenGLTexture::Target2D);
    waveform->setFormat(QOpenGLTexture::R32F);
    waveform->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    updateWaveformTexture();
}

void WaveformWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    program->setUniformValue("uMarkerWidth", data.markerWidth / width());
    timer.stop();
    timer.start(data.redrawOnResizeTimeout);
}

void WaveformWidget::paintGL() {
    glClearColor(1.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    program->bind();
    if (cursorChanged) program->setUniformValue("uCursor", WaveformWidget::cursor);
    if (focusChanged) program->setUniformValue("uFocusWindow", WaveformWidget::focusMarkers);
    if (markersChanged) {
        for (int i = 0; i < MAX_MARKER_COUNT; ++i) {
            std::string name = "uMarkers[" + std::to_string(i) + "]";
            program->setUniformValue(name.c_str(), markers[i]);
        }
    }
    waveform->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
}

void WaveformWidget::updateResize() {
    std::cout << "New Size: " << width() << std::endl;
    generator.setWidth(width());
    if (generator.isInvalidated()) {
        updateWaveformTexture();
        repaint();
    }
}

void WaveformWidget::updateWaveformTexture() {
    generator.recalculate();
    waveform->destroy();
    waveform->setFormat(QOpenGLTexture::R32F);
    waveform->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    waveform->setSize(ACI(generator.getOutput().n_rows), 2);
    waveform->allocateStorage();
    waveform->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, generator.getOutput().memptr());
}

QString WaveformWidget::getFragmentShader() {
    return ":shaders/waveform.fs.glsl";
}

void WaveformWidget::initDefaultUniform() {
    program->setUniformValue("uTexture", 0);
    program->setUniformValue("uBackgroundColor", palette().color(QPalette::Background));
    program->setUniformValue("uPrimaryColor", palette().color(QPalette::Highlight));
    program->setUniformValue("uSecondaryColor", palette().color(QPalette::Light));
    program->setUniformValue("uFocusBorderColor", data.markerColor);
    program->setUniformValue("uMarkerBorderColor", data.focusMarkerColor);
    program->setUniformValue("uCursorBorderColor", data.cursorMarkerColor);
    program->setUniformValue("uMarkerWidth", data.markerWidth / width());
    markers.fill(-1.f);
}

void WaveformWidget::setCursor(int cursor) {
    WaveformWidget::cursor = cursor / (float) input.size();
    cursorChanged = true;
}

void WaveformWidget::setFocusMarkers(int start, int end) {
    WaveformWidget::focusMarkers = QVector2D(start / (float) input.size(), end / (float) input.size());
    focusChanged = true;
}

int WaveformWidget::setMarker(int pos) {
    for (int i = 0; i < MAX_MARKER_COUNT; ++i) {
        if (markers[i] > -1.f) continue;
        markers[i] = pos / (float) input.size();
        markersChanged = true;
        return i;
    }
    return -1;
}

void WaveformWidget::removeMarker(int idx) {
    if (idx >= 0 && idx < MAX_MARKER_COUNT) {
        markers[idx] = -1.f;
        markersChanged = true;
    }
}



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
        struct WaveformWidgetData {
            float markerWidth = 1.f;
            QColor focusMarkerColor = QColor(255, 209, 102);
            QColor cursorMarkerColor = QColor(0, 148, 198);
            QColor markerColor = QColor(239, 111, 108);
            int redrawOnResizeTimeout = 1000;
        } data;

        explicit WaveformWidget(QWidget *parent = nullptr);

        void setCursor(int cursor);

        void setFocusMarkers(int start, int end);

        int setMarker(int pos);

        void removeMarker(int idx);

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
        static const int MAX_MARKER_COUNT = 10;

        QTimer timer;

        QOpenGLShaderProgram *program;
        QOpenGLVertexArrayObject vao;
        QOpenGLBuffer vbo;
        Mesh<Vertex2D> quad;

        QOpenGLTexture *waveform;
        WaveformGenerator generator;
        fvec input;

        bool cursorChanged = true, focusChanged = true, markersChanged = true;
        float cursor = 0;
        QVector2D focusMarkers = {-1, 2};
        std::array<float, MAX_MARKER_COUNT> markers;
    };
}

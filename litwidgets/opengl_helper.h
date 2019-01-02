//
// Created by egordm on 2-1-19.
//

#pragma once

#include <QVector3D>
#include <QVector2D>

namespace litwidgets {
    struct Vertex2D {
        QVector3D position;
        QVector2D uv;

        Vertex2D(const QVector3D &position, const QVector2D &uv) : position(position), uv(uv) {}
    };

    template <typename T>
    class Mesh {
        std::vector<T> vertex_data;

    public:
        Mesh() {}

        Mesh(const std::vector<T> &vertices) : vertex_data(vertices) {}

        void *getVertexData() {
            return vertex_data.data();
        }

        int getVertexDataSize() {
            return static_cast<int>(vertex_data.size() * sizeof(T));
        }

        int getVertexCount() const {
            return static_cast<int>(vertex_data.size());
        }
    };

    inline Mesh<Vertex2D> create_quad() {
        std::vector<Vertex2D> vertices;
        vertices.push_back({{1.f, 1.f, 0}, {1, 0}});
        vertices.push_back({{1.f, -1.f, 0}, {1, 1}});
        vertices.push_back({{-1.f, 1.f, 0}, {0, 0}});
        vertices.push_back({{1.f, -1.f, 0}, {1, 1}});
        vertices.push_back({{-1.f, -1.f, 0}, {0, 1}});
        vertices.push_back({{-1.f, 1.f, 0}, {0, 0}});

        return Mesh<Vertex2D>(vertices);
    }
}



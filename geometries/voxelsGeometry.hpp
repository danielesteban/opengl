#pragma once

#include "chunks.hpp"
#include "geometry.hpp"

class VoxelsGeometry : public Geometry {
  public:
    VoxelsGeometry();
    void generate(Chunks *chunks, const GLint cx, const GLint cy, const GLint cz);
  private:
    typedef struct {
      GLfloat x, y, z,  r, g, b,  u, v;
    } Vertex;
    static const Voxel air;
    static const Voxel bedrock;
    static const glm::vec2 uv[4];
};

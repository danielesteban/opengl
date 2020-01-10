#pragma once

#include "chunks.hpp"
#include "geometry.hpp"

class VoxelsGeometry : public Geometry {
  public:
    VoxelsGeometry(Chunks *chunks, const GLint cx, const GLint cy, const GLint cz);
  private:
    static const Voxel air;
    static const Voxel bedrock;
    static const glm::vec2 VoxelsGeometry::uv[4];
};

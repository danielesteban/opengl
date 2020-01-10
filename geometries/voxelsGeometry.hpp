#pragma once

#include "chunks.hpp"
#include "geometry.hpp"

class VoxelsGeometry : public Geometry {
  public:
    VoxelsGeometry(Chunks *chunks, const GLint x, const GLint y, const GLint z);
  private:
    static const GLfloat ao(const bool n1, const bool n2, const bool n3);
};

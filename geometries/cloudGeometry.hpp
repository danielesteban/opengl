#pragma once

#include "geometry.hpp"

class CloudGeometry : public Geometry {
  public:
    CloudGeometry();
  private:
    typedef struct {
      GLfloat x, y, z,  r, g, b;
    } Vertex;
};

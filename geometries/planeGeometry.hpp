#pragma once

#include "geometry.hpp"

class PlaneGeometry : public Geometry {
  public:
    PlaneGeometry(const GLfloat width, const GLfloat height);
};

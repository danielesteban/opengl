#pragma once

#include "geometry.hpp"

class BoxGeometry : public Geometry {
  public:
    BoxGeometry(const GLfloat width, const GLfloat height, const GLfloat length);
};

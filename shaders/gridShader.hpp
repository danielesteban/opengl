#pragma once

#include "shader.hpp"

class GridShader : public Shader {
  public:
    GridShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

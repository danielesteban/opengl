#pragma once

#include "shader.hpp"

class CubeShader : public Shader {
  public:
    CubeShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

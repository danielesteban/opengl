#pragma once

#include "shader.hpp"

class OceanShader : public Shader {
  public:
    OceanShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

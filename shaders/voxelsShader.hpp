#pragma once

#include "shader.hpp"

class VoxelsShader : public Shader {
  public:
    VoxelsShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

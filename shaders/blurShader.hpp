#pragma once

#include "shader.hpp"

class BlurShader : public Shader {
  public:
    BlurShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

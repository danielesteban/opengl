#pragma once

#include "shader.hpp"

class BasicShader : public Shader {
  public:
    BasicShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

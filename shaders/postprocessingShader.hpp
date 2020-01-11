#pragma once

#include "shader.hpp"

class PostprocessingShader : public Shader {
  public:
    PostprocessingShader();
  private:
    static const char *fragmentSource;
    static const char *vertexSource;
};

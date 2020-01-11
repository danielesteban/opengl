#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
  public:
    Texture();
    ~Texture();
    GLuint texture;
    void bind();
};

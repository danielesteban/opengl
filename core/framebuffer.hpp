#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "planeGeometry.hpp"

class Framebuffer {
  public:
    Framebuffer(Shader *shader, const GLint samples = 4);
    ~Framebuffer();
    void bind();
    void resize(GLint width, GLint height);
    void render();
  private:
    GLuint fbo;
    GLuint rbo;
    PlaneGeometry screen;
    GLuint texture;
    Shader *shader;
    const GLuint samples;
};

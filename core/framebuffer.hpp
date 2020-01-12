#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer {
  public:
    Framebuffer(const bool depth, const GLint samples = 4);
    ~Framebuffer();
    void bind();
    void bindColorTexture();
    void bindDepthTexture();
    void resize(GLint width, GLint height);
  private:
    GLuint colorTexture;
    GLuint depthTexture;
    GLuint fbo;
    const bool depth;
    const GLuint samples;
};

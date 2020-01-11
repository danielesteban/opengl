#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer {
  public:
    Framebuffer(const bool depth = false, const bool multisampled = false);
    ~Framebuffer();
    GLint width;
    GLint height;
    void bind();
    void bindDraw();
    void bindRead();
    void bindTexture();
    void resize(GLint width, GLint height);
  private:
    const bool depth;
    const bool multisampled;
    GLuint fbo;
    GLuint rbo;
    GLuint texture;
    static const GLuint numSamples;
};

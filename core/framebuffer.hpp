#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer {
  public:
    Framebuffer(const bool depth, const bool multisampled, const GLint samples);
    ~Framebuffer();
    GLint width, height;
    void bind();
    void bindColorTexture();
    void bindDepthTexture();
    void bindDraw();
    void bindRead();
    void resize(GLint width, GLint height);
  private:
    GLuint colorTexture;
    GLuint depthTexture;
    GLuint fbo;
    const bool depth;
    const bool multisampled;
    const GLuint samples;
};

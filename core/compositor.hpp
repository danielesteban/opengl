#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "framebuffer.hpp"
#include "planeGeometry.hpp"
#include "blurShader.hpp"
#include "postprocessingShader.hpp"

class Compositor {
  public:
    Compositor();
    void initFrame();
    void renderFrame();
    void resize(GLint width, GLint height);
  private:
    BlurShader blurShader;
    PostprocessingShader postprocessingShader;
    Framebuffer colorBufferA;
    Framebuffer colorBufferB;
    Framebuffer renderBuffer;
    PlaneGeometry screen;
    static const GLint samples;
};

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"

class Shader {
  public:
    Shader(const char *vertexSource, const char *fragmentSource);
    void use();
    void updateAlbedo(const GLfloat *color);
    void updateCamera(const Camera *camera);
    void updateModel(const GLfloat *matrix);
  private:
    GLuint program;
    GLuint fragmentShader;
    GLuint vertexShader;
    struct {
      GLuint albedo, model, projection, view;
    } uniforms;
};

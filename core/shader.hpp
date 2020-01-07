#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.hpp"

class Shader {
  public:
    Shader(const char *vertexSource, const char *fragmentSource);
    void use();
    void updateAlbedo(const glm::vec3 &color);
    void updateCamera(const Camera &camera);
    void updateModel(const glm::mat4 &matrix);
  private:
    GLuint program;
    GLuint fragmentShader;
    GLuint vertexShader;
    struct {
      GLuint albedo, model, projection, view;
    } uniforms;
};

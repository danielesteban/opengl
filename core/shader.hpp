#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.hpp"

class Shader {
  public:
    Shader(const char *vertexSource, const char *fragmentSource);
    ~Shader();
    void use();
    void updateAlbedo(const glm::vec3 &color);
    void updateCamera(const Camera &camera);
    void updateFlag(const bool status);
    void updateFog(const glm::vec3 &color, const GLfloat density);
    void updateModel(const glm::mat4 &matrix);
    void updateResolution(const glm::vec2 &resolution);
    void updateSamples(const GLint count);
  private:
    GLuint program;
    GLuint fragmentShader;
    GLuint vertexShader;
    struct {
      GLuint albedo, flag, fogColor, fogDensity, model, projection, view, resolution, samples;
    } uniforms;
};

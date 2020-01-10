#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#ifndef NDEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

Shader::Shader(const char *vertexSource, const char *fragmentSource) {
  #ifndef NDEBUG
  GLchar infoLog[512];
  #endif
  GLint success;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    #ifndef NDEBUG
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);
    #endif
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    #ifndef NDEBUG
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);
    #endif
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    #ifndef NDEBUG
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    fprintf(stderr, "Shader Program Error: %s\n", infoLog);
    #endif
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  uniforms.albedo = glGetUniformLocation(program, "albedo");
  uniforms.fogColor = glGetUniformLocation(program, "fogColor");
  uniforms.fogDensity = glGetUniformLocation(program, "fogDensity");
  uniforms.model = glGetUniformLocation(program, "model");
  uniforms.projection = glGetUniformLocation(program, "projection");
  uniforms.view = glGetUniformLocation(program, "view");
}

Shader::~Shader() {
  glDeleteProgram(program);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
}

void Shader::use() {
  glUseProgram(program);
}

void Shader::updateAlbedo(const glm::vec3 &color) {
  if (uniforms.albedo != -1) {
    glUniform3fv(uniforms.albedo, 1, glm::value_ptr(color));
  }
}

void Shader::updateCamera(const Camera &camera) {
  if (uniforms.projection != -1) {
    glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
  }
  if (uniforms.view != -1) {
    glUniformMatrix4fv(uniforms.view, 1, GL_FALSE, glm::value_ptr(camera.view));
  }
}

void Shader::updateFog(const glm::vec3 &color, const GLfloat density) {
  if (uniforms.fogColor != -1) {
    glUniform3fv(uniforms.fogColor, 1, glm::value_ptr(color));
  }
  if (uniforms.fogDensity != -1) {
    glUniform1f(uniforms.fogDensity, density);
  }
}

void Shader::updateModel(const glm::mat4 &matrix) {
  if (uniforms.model != -1) {
    glUniformMatrix4fv(uniforms.model, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}

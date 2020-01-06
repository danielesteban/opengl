#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Geometry {
  public:
    Geometry();
    glm::vec3 origin;
    GLfloat radius;
    void draw();
  protected:
    GLuint count;
    GLuint ebo;
    GLuint vao;
    GLuint vbo;
};

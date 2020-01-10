#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Geometry {
  public:
    Geometry();
    ~Geometry();
    GLuint count;
    glm::vec3 origin;
    GLfloat radius;
    void draw();
  protected:
    GLuint ebo;
    GLuint vao;
    GLuint vbo;
};

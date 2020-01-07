#include "boxGeometry.hpp"

BoxGeometry::BoxGeometry(const GLfloat width, const GLfloat height, const GLfloat length) {
  const GLfloat hw = width * 0.5f;
  const GLfloat hh = height * 0.5f;
  const GLfloat hl = length * 0.5f;
  const struct {
    GLfloat x, y, z,  r, g, b;
  } vertices[] = {
    {-hw, -hh, hl,    0.5f, 0.5f, 0.5f},
    {hw, -hh, hl,     0.5f, 0.5f, 0.5f},
    {hw, hh, hl,      0.5f, 0.5f, 0.5f},
    {-hw, hh, hl,     0.5f, 0.5f, 0.5f},

    {hw, -hh, hl,     0.7f, 0.7f, 0.7f},
    {hw, -hh, -hl,    0.7f, 0.7f, 0.7f},
    {hw, hh, -hl,     0.7f, 0.7f, 0.7f},
    {hw, hh, hl,      0.7f, 0.7f, 0.7f},

    {hw, -hh, -hl,    0.4f, 0.4f, 0.4f},
    {-hw, -hh, -hl,   0.4f, 0.4f, 0.4f},
    {-hw, hh, -hl,    0.4f, 0.4f, 0.4f},
    {hw, hh, -hl,     0.4f, 0.4f, 0.4f},

    {-hw, -hh, -hl,   0.3f, 0.3f, 0.3f},
    {-hw, -hh, hl,    0.3f, 0.3f, 0.3f},
    {-hw, hh, hl,     0.3f, 0.3f, 0.3f},
    {-hw, hh, -hl,    0.3f, 0.3f, 0.3f},

    {-hw, -hh, -hl,   0.2f, 0.2f, 0.2f},
    {hw, -hh, -hl,    0.2f, 0.2f, 0.2f},
    {hw, -hh, hl,     0.2f, 0.2f, 0.2f},
    {-hw, -hh, hl,    0.2f, 0.2f, 0.2f},

    {-hw, hh, hl,     0.8f, 0.8f, 0.8f},
    {hw, hh, hl,      0.8f, 0.8f, 0.8f},
    {hw, hh, -hl,     0.8f, 0.8f, 0.8f},
    {-hw, hh, -hl,    0.8f, 0.8f, 0.8f},
  };
  const GLushort indices[] = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 11, 8,
    12, 13, 14,
    14, 15, 12,
    16, 17, 18,
    18, 19, 16,
    20, 21, 22,
    22, 23, 20
  };
  const GLfloat side = fmax(hw, fmax(hh, hl));
  count = sizeof(indices) / sizeof(GLushort);
  origin = glm::vec3(0.0f, 0.0f, 0.0f);
  radius = sqrt(side * side + side * side);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *) (sizeof(GLfloat) * 3));
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

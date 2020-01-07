#include "planeGeometry.hpp"

PlaneGeometry::PlaneGeometry(const GLfloat width, const GLfloat height) {
  const GLfloat hw = width * 0.5f;
  const GLfloat hh = height * 0.5f;
  const struct {
    GLfloat x, y, z;
  } vertices[] = {
    {-hw, -hh, 0.0f},
    {hw, -hh, 0.0f},
    {hw, hh, 0.0f},
    {-hw, hh, 0.0f},
  };
  const GLushort indices[] = {
    0, 1, 2,
    2, 3, 0
  };
  const GLfloat side = fmax(hw, hh);
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
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
}

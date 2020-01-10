#include "geometry.hpp"

Geometry::Geometry() {
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
}

Geometry::~Geometry() {
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Geometry::draw() {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}

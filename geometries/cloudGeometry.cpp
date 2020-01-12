#include "cloudGeometry.hpp"
#include "FastNoise.h"
#include <vector>

CloudGeometry::CloudGeometry() {
  std::vector<GLushort> indices;
  std::vector<Vertex> vertices;
  struct {
    glm::vec3 color;
    glm::vec3 vertices[4];
  } face;
  FastNoise noise;
  GLushort offset = 0;
  
  auto pushFace = [&indices, &vertices, &offset, &face]() {
    for (GLint i = 0; i < 4; i += 1) {
      vertices.push_back({
        face.vertices[i].x, face.vertices[i].y, face.vertices[i].z,
        face.color.x, face.color.y, face.color.z,
      });
    }
    indices.push_back(offset);
    indices.push_back(offset + 1);
    indices.push_back(offset + 2);
    indices.push_back(offset + 2);
    indices.push_back(offset + 3);
    indices.push_back(offset);
    offset += 4;
  };

  noise.SetNoiseType(FastNoise::SimplexFractal);
  noise.SetFrequency(0.005f);
  noise.SetSeed(rand());

  const GLint cols = 16 + (GLint) floor(((GLfloat) rand() / RAND_MAX) * 8.0f);
  const GLint rows = (GLint) (
    (GLfloat) cols * (1.0f + (((GLfloat) rand() / RAND_MAX) * 0.5f) - 0.25f)
  );

  const GLfloat width = (GLfloat) cols / 2.5f;
  const GLfloat height = 1.0f;
  const GLfloat length = (GLfloat) rows / 2.5f;

  bool **bitmap = new bool*[rows];
  const GLfloat len = fmax((GLfloat) cols * 0.5f, (GLfloat) rows * 0.5f);
  const glm::vec2 center = glm::vec2(
    (GLfloat) cols * 0.5f,
    (GLfloat) rows * 0.5f
  );
  for (GLint z = 0; z < rows; z += 1) {
    bitmap[z] = new bool[cols];
    for (GLint x = 0; x < cols; x += 1) {
      bitmap[z][x] = (
        glm::distance(center, glm::vec2((GLfloat) x, (GLfloat) z)) <= len
        && noise.GetNoise(
          (GLfloat) x * cols,
          (GLfloat) z * rows
        ) < 0.0f
      );
    }
  }

  auto test = [&bitmap, &cols, &rows](GLint z, GLint x) {
    if (
      z < 0
      || z >= rows
      || x < 0
      || x >= cols
    ) {
      return false;
    }
    return bitmap[z][x];
  };

  for (GLint z = -1; z <= rows; z += 1) {
    for (GLint x = -1; x <= cols; x += 1) {
      const bool p = test(z, x);
      const bool pX = test(z, x + 1);
      const bool pZ = test(z + 1, x);
      GLfloat vX = ((GLfloat) x * width * 2.0f) - (width * (GLfloat) cols * 0.5f);
      GLfloat vZ = ((GLfloat) z * length * 2.0f) - (length * (GLfloat) rows * 0.5f);
      if (p) {
        // BOTTOM
        face.color = glm::vec3(0.9f, 0.9f, 0.9f);
        face.vertices[0] = glm::vec3(vX - width, -height, vZ - length);
        face.vertices[1] = glm::vec3(vX + width, -height, vZ - length);
        face.vertices[2] = glm::vec3(vX + width, -height, vZ + length);
        face.vertices[3] = glm::vec3(vX - width, -height, vZ + length);
        pushFace();
        if (!pX) {
          // WEST
          face.color = glm::vec3(1.0f, 1.0f, 1.0f);
          face.vertices[0] = glm::vec3(vX + width, -height, vZ + length);
          face.vertices[1] = glm::vec3(vX + width, -height, vZ - length);
          face.vertices[2] = glm::vec3(vX + width, height, vZ - length);
          face.vertices[3] = glm::vec3(vX + width, height, vZ + length);
          pushFace();
        }
        if (!pZ) {
          // SOUTH
          face.color = glm::vec3(1.0f, 1.0f, 1.0f);
          face.vertices[0] = glm::vec3(vX - width, -height, vZ + length);
          face.vertices[1] = glm::vec3(vX + width, -height, vZ + length);
          face.vertices[2] = glm::vec3(vX + width, height, vZ + length);
          face.vertices[3] = glm::vec3(vX - width, height, vZ + length);
          pushFace();
        }
      } else {
        if (pX) {
          // EAST
          face.color = glm::vec3(1.0f, 1.0f, 1.0f);
          vX += width * 2.0f;
          face.vertices[0] = glm::vec3(vX - width, -height, vZ - length);
          face.vertices[1] = glm::vec3(vX - width, -height, vZ + length);
          face.vertices[2] = glm::vec3(vX - width, height, vZ + length);
          face.vertices[3] = glm::vec3(vX - width, height, vZ - length);
          vX -= width * 2.0f;
          pushFace();
        }
        if (pZ) {
          // NORTH
          face.color = glm::vec3(1.0f, 1.0f, 1.0f);
          vZ += length * 2.0f;
          face.vertices[0] = glm::vec3(vX + width, -height, vZ - length);
          face.vertices[1] = glm::vec3(vX - width, -height, vZ - length);
          face.vertices[2] = glm::vec3(vX - width, height, vZ - length);
          face.vertices[3] = glm::vec3(vX + width, height, vZ - length);
          vZ -= length * 2.0f;
          pushFace();
        }
      }
    }
  }

  for (GLint z = 0; z < rows; z += 1) {
    delete [] bitmap[z];
  }
  delete [] bitmap;

  const GLfloat side = fmax(width * (GLfloat) cols, fmax(length * (GLfloat) rows, height));
  origin = glm::vec3(0.0f, 0.0f, 0.0f);
  radius = sqrt(side * side + side * side);
  count = (GLuint) indices.size();

  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(GLfloat) * 3));
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

#include "voxelsGeometry.hpp"

const GLfloat VoxelsGeometry::ao(bool n[]) {
  GLfloat light = 1.0f;
  if (n[0]) {
    light -= 0.2f;
  }
  if (n[1]) {
    light -= 0.2f;
  }
  if ((n[0] && n[1]) || n[2]) {
    light -= 0.2f;
  }
  return light;
}

VoxelsGeometry::VoxelsGeometry(Chunks *chunks, const GLint x, const GLint y, const GLint z) {
  typedef struct {
    GLfloat x, y, z,  r, g, b,  u, v;
  } Vertex;
  std::vector<Vertex> vertices;
  std::vector<GLushort> indices;

  struct {
    GLint x, y, z;
    bool n[3];
  } points[4];

  GLushort o = 0;
  auto pushFace = [&vertices, &indices, &points, &o](const GLubyte type, const glm::vec3 &color) {
    const GLfloat light[4] = {
      ao(points[0].n),
      ao(points[1].n),
      ao(points[2].n),
      ao(points[3].n)
    };
    const glm::vec2 uv[4] = {
      {(GLfloat) type, 0.0f},
      {(GLfloat) type + 1.0f, 0.0f},
      {(GLfloat) type + 1.0f, 1.0f},
      {(GLfloat) type, 1.0f}
    };
    const GLint v = (light[0] + light[2] < light[1] + light[3]) ? 1 : 0;
    for (GLint p = 0; p < 4; p += 1) {
      GLint i = (v + p) % 4;
      vertices.push_back({
        (GLfloat) points[i].x, (GLfloat) points[i].y, (GLfloat) points[i].z,
        color.x * light[i], color.y * light[i], color.z * light[i],
        uv[i].x, uv[i].y
      });
    }
    indices.push_back(o);
    indices.push_back(o + 1);
    indices.push_back(o + 2);
    indices.push_back(o + 2);
    indices.push_back(o + 3);
    indices.push_back(o);
    o += 4;
  };

  const GLint cx = x * ChunkSize;
  const GLint cy = y * ChunkSize;
  const GLint cz = z * ChunkSize;
  for (GLint z = 0; z < ChunkSize; z++) {
    const GLint vz = cz + z;
    for (GLint y = 0; y < ChunkSize; y++) {
      const GLint vy = cy + y;
      for (GLint x = 0; x < ChunkSize; x++) {
        const GLint vx = cx + x;
        const Voxel *voxel = chunks->getVoxel(vx, vy, vz);
        if (voxel->type != 0) {
          // TOP
          if (!chunks->test(vx, vy + 1, vz)) {
            const bool n = chunks->test(vx, vy + 1, vz - 1);
            const bool e = chunks->test(vx + 1, vy + 1, vz);
            const bool w = chunks->test(vx - 1, vy + 1, vz);
            const bool s = chunks->test(vx, vy + 1, vz + 1);
            points[0] = {x, y + 1, z + 1,         w, s, chunks->test(vx - 1, vy + 1, vz + 1)};
            points[1] = {x + 1, y + 1, z + 1,     e, s, chunks->test(vx + 1, vy + 1, vz + 1)};
            points[2] = {x + 1, y + 1, z,         e, n, chunks->test(vx + 1, vy + 1, vz - 1)};
            points[3] = {x, y + 1, z,             w, n, chunks->test(vx - 1, vy + 1, vz - 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
          // BOTTOM
          if (!chunks->test(vx, vy - 1, vz)) {
            const bool n = chunks->test(vx, vy - 1, vz - 1);
            const bool e = chunks->test(vx + 1, vy - 1, vz);
            const bool w = chunks->test(vx - 1, vy - 1, vz);
            const bool s = chunks->test(vx, vy - 1, vz + 1);
            points[0] = {x, y, z,                 w, n, chunks->test(vx - 1, vy - 1, vz - 1)};
            points[1] = {x + 1, y, z,             e, n, chunks->test(vx + 1, vy - 1, vz - 1)};
            points[2] = {x + 1, y, z + 1,         e, s, chunks->test(vx + 1, vy - 1, vz + 1)};
            points[3] = {x, y, z + 1,             w, s, chunks->test(vx - 1, vy - 1, vz + 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
          // SOUTH
          if (!chunks->test(vx, vy, vz + 1)) {
            const bool e = chunks->test(vx + 1, vy, vz + 1);
            const bool w = chunks->test(vx - 1, vy, vz + 1);
            const bool t = chunks->test(vx, vy + 1, vz + 1);
            const bool b = chunks->test(vx, vy - 1, vz + 1);
            points[0] = {x, y, z + 1,             w, b, chunks->test(vx - 1, vy - 1, vz + 1)};
            points[1] = {x + 1, y, z + 1,         e, b, chunks->test(vx + 1, vy - 1, vz + 1)};
            points[2] = {x + 1, y + 1, z + 1,     e, t, chunks->test(vx + 1, vy + 1, vz + 1)};
            points[3] = {x, y + 1, z + 1,         w, t, chunks->test(vx - 1, vy + 1, vz + 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
          // NORTH
          if (!chunks->test(vx, vy, vz - 1)) {
            const bool e = chunks->test(vx + 1, vy, vz - 1);
            const bool w = chunks->test(vx - 1, vy, vz - 1);
            const bool t = chunks->test(vx, vy + 1, vz - 1);
            const bool b = chunks->test(vx, vy - 1, vz - 1);
            points[0] = {x + 1, y, z,             e, b, chunks->test(vx + 1, vy - 1, vz - 1)};
            points[1] = {x, y, z,                 w, b, chunks->test(vx - 1, vy - 1, vz - 1)};
            points[2] = {x, y + 1, z,             w, t, chunks->test(vx - 1, vy + 1, vz - 1)};
            points[3] = {x + 1, y + 1, z,         e, t, chunks->test(vx + 1, vy + 1, vz - 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
          // WEST
          if (!chunks->test(vx + 1, vy, vz)) {
            const bool n = chunks->test(vx + 1, vy, vz - 1);
            const bool s = chunks->test(vx + 1, vy, vz + 1);
            const bool t = chunks->test(vx + 1, vy + 1, vz);
            const bool b = chunks->test(vx + 1, vy - 1, vz);
            points[0] = {x + 1, y, z + 1,         s, b, chunks->test(vx + 1, vy - 1, vz + 1)};
            points[1] = {x + 1, y, z,             n, b, chunks->test(vx + 1, vy - 1, vz - 1)};
            points[2] = {x + 1, y + 1, z,         n, t, chunks->test(vx + 1, vy + 1, vz - 1)};
            points[3] = {x + 1, y + 1, z + 1,     s, t, chunks->test(vx + 1, vy + 1, vz + 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
          // EAST
          if (!chunks->test(vx - 1, vy, vz)) {
            const bool n = chunks->test(vx - 1, vy, vz - 1);
            const bool s = chunks->test(vx - 1, vy, vz + 1);
            const bool t = chunks->test(vx - 1, vy + 1, vz);
            const bool b = chunks->test(vx - 1, vy - 1, vz);
            points[0] = {x, y, z,                 n, b, chunks->test(vx - 1, vy - 1, vz - 1)};
            points[1] = {x, y, z + 1,             s, b, chunks->test(vx - 1, vy - 1, vz + 1)};
            points[2] = {x, y + 1, z + 1,         s, t, chunks->test(vx - 1, vy + 1, vz + 1)};
            points[3] = {x, y + 1, z,             n, t, chunks->test(vx - 1, vy + 1, vz - 1)};
            pushFace(voxel->type - 1, voxel->color);
          }
        }
      }
    }
  }

  count = (GLuint) indices.size();
  if (count > 0) {
    const GLfloat side = ChunkSize * 0.5f;
    origin = glm::vec3(side, side, side);
    radius = sqrt(side * side + side * side);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(GLfloat) * 6));
  }
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

#include "voxelsGeometry.hpp"

const Voxel VoxelsGeometry::air = {0, { 0.0f, 0.0f, 0.0f }};
const Voxel VoxelsGeometry::bedrock = {0xFF, { 0.0f, 0.0f, 0.0f }};
const glm::vec2 VoxelsGeometry::uv[4] = {
  {0.0f, 0.0f},
  {1.0f, 0.0f},
  {1.0f, 1.0f},
  {0.0f, 1.0f},
};

VoxelsGeometry::VoxelsGeometry() {
  const GLfloat side = ChunkSize * 0.5f;
  origin = glm::vec3(side, side, side);
  radius = sqrt(side * side + side * side);
}

void VoxelsGeometry::generate(Chunks *chunks, const GLint cx, const GLint cy, const GLint cz) {
  const Chunk *neighbours[3][3] = {
    {chunks->get(cx - 1, cz - 1), chunks->get(cx, cz - 1), chunks->get(cx + 1, cz - 1)},
    {chunks->get(cx - 1, cz), chunks->get(cx, cz), chunks->get(cx + 1, cz)},
    {chunks->get(cx - 1, cz + 1), chunks->get(cx, cz + 1), chunks->get(cx + 1, cz + 1)}
  };
  const GLint yOffset = cy * ChunkSize;
  
  std::vector<GLushort> indices;
  std::vector<Vertex> vertices;
  GLushort offset = 0;
  struct {
    GLint x, y, z;
    GLfloat light;
  } points[4];

  auto ao = [](const bool n1, const bool n2, const bool n3) {
    GLfloat light = 1.0f;
    if (n1) {
      light -= 0.2f;
    }
    if (n2) {
      light -= 0.2f;
    }
    if ((n1 && n2) || n3) {
      light -= 0.2f;
    }
    return light;
  };

  auto get = [&neighbours, &yOffset](GLint x, GLint y, GLint z) {
    y += yOffset;
    if (y < 0) {
      return &bedrock;
    }
    if (y >= ChunkSize * NumSubchunks) {
      return &air;
    }
    GLint chunkX = 1;
    GLint chunkZ = 1;
    if (x < 0) {
      chunkX -= 1;
      x += ChunkSize;
    } else if (x >= ChunkSize) {
      chunkX += 1;
      x -= ChunkSize;
    }
    if (z < 0) {
      chunkZ -= 1;
      z += ChunkSize;
    } else if (z >= ChunkSize) {
      chunkZ += 1;
      z -= ChunkSize;
    }
    return &neighbours[chunkZ][chunkX]->voxels[
      (z * ChunkSize * ChunkSize * NumSubchunks) + (y * ChunkSize) + x
    ];
  };

  auto pushFace = [&indices, &vertices, &offset, &points](const GLubyte type, const glm::vec3 &color) {
    const GLint v = (points[0].light + points[2].light < points[1].light + points[3].light) ? 1 : 0;
    for (GLint p = 0; p < 4; p += 1) {
      GLint i = (v + p) % 4;
      vertices.push_back({
        (GLfloat) points[i].x, (GLfloat) points[i].y, (GLfloat) points[i].z,
        color.x * points[i].light, color.y * points[i].light, color.z * points[i].light,
        uv[i].x + (GLfloat) type, uv[i].y
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

  auto test = [&get](const GLint x, const GLint y, const GLint z) {
    return get(x, y, z)->type != 0;
  };

  for (GLint z = 0; z < ChunkSize; z++) {
    for (GLint y = 0; y < ChunkSize; y++) {
      for (GLint x = 0; x < ChunkSize; x++) {
        const Voxel *voxel = get(x, y, z);
        if (voxel->type != 0) {
          // TOP
          if (!test(x, y + 1, z)) {
            const bool n = test(x, y + 1, z - 1);
            const bool e = test(x + 1, y + 1, z);
            const bool w = test(x - 1, y + 1, z);
            const bool s = test(x, y + 1, z + 1);
            points[0] = {x, y + 1, z + 1,       ao(w, s, test(x - 1, y + 1, z + 1))};
            points[1] = {x + 1, y + 1, z + 1,   ao(e, s, test(x + 1, y + 1, z + 1))};
            points[2] = {x + 1, y + 1, z,       ao(e, n, test(x + 1, y + 1, z - 1))};
            points[3] = {x, y + 1, z,           ao(w, n, test(x - 1, y + 1, z - 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
          // BOTTOM
          if (!test(x, y - 1, z)) {
            const bool n = test(x, y - 1, z - 1);
            const bool e = test(x + 1, y - 1, z);
            const bool w = test(x - 1, y - 1, z);
            const bool s = test(x, y - 1, z + 1);
            points[0] = {x, y, z,               ao(w, n, test(x - 1, y - 1, z - 1))};
            points[1] = {x + 1, y, z,           ao(e, n, test(x + 1, y - 1, z - 1))};
            points[2] = {x + 1, y, z + 1,       ao(e, s, test(x + 1, y - 1, z + 1))};
            points[3] = {x, y, z + 1,           ao(w, s, test(x - 1, y - 1, z + 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
          // SOUTH
          if (!test(x, y, z + 1)) {
            const bool e = test(x + 1, y, z + 1);
            const bool w = test(x - 1, y, z + 1);
            const bool t = test(x, y + 1, z + 1);
            const bool b = test(x, y - 1, z + 1);
            points[0] = {x, y, z + 1,           ao(w, b, test(x - 1, y - 1, z + 1))};
            points[1] = {x + 1, y, z + 1,       ao(e, b, test(x + 1, y - 1, z + 1))};
            points[2] = {x + 1, y + 1, z + 1,   ao(e, t, test(x + 1, y + 1, z + 1))};
            points[3] = {x, y + 1, z + 1,       ao(w, t, test(x - 1, y + 1, z + 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
          // NORTH
          if (!test(x, y, z - 1)) {
            const bool e = test(x + 1, y, z - 1);
            const bool w = test(x - 1, y, z - 1);
            const bool t = test(x, y + 1, z - 1);
            const bool b = test(x, y - 1, z - 1);
            points[0] = {x + 1, y, z,           ao(e, b, test(x + 1, y - 1, z - 1))};
            points[1] = {x, y, z,               ao(w, b, test(x - 1, y - 1, z - 1))};
            points[2] = {x, y + 1, z,           ao(w, t, test(x - 1, y + 1, z - 1))};
            points[3] = {x + 1, y + 1, z,       ao(e, t, test(x + 1, y + 1, z - 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
          // WEST
          if (!test(x + 1, y, z)) {
            const bool n = test(x + 1, y, z - 1);
            const bool s = test(x + 1, y, z + 1);
            const bool t = test(x + 1, y + 1, z);
            const bool b = test(x + 1, y - 1, z);
            points[0] = {x + 1, y, z + 1,       ao(s, b, test(x + 1, y - 1, z + 1))};
            points[1] = {x + 1, y, z,           ao(n, b, test(x + 1, y - 1, z - 1))};
            points[2] = {x + 1, y + 1, z,       ao(n, t, test(x + 1, y + 1, z - 1))};
            points[3] = {x + 1, y + 1, z + 1,   ao(s, t, test(x + 1, y + 1, z + 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
          // EAST
          if (!test(x - 1, y, z)) {
            const bool n = test(x - 1, y, z - 1);
            const bool s = test(x - 1, y, z + 1);
            const bool t = test(x - 1, y + 1, z);
            const bool b = test(x - 1, y - 1, z);
            points[0] = {x, y, z,               ao(n, b, test(x - 1, y - 1, z - 1))};
            points[1] = {x, y, z + 1,           ao(s, b, test(x - 1, y - 1, z + 1))};
            points[2] = {x, y + 1, z + 1,       ao(s, t, test(x - 1, y + 1, z + 1))};
            points[3] = {x, y + 1, z,           ao(n, t, test(x - 1, y + 1, z - 1))};
            pushFace(voxel->type - 1, voxel->color);
          }
        }
      }
    }
  }

  count = (GLuint) indices.size();
  if (count != 0) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(GLfloat) * 6));
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
}

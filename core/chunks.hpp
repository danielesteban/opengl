#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "FastNoise.h"

#define ChunkSize 16
#define NumSubchunks 4

typedef struct {
  GLubyte type;
  glm::vec3 color;
} Voxel;

typedef struct {
  GLubyte heightmap[ChunkSize * ChunkSize];
  Voxel voxels[ChunkSize * ChunkSize * ChunkSize * NumSubchunks];
} Chunk;

class Chunks {
  public:
    Chunks();
    const Chunk* get(const GLint x, const GLint z);
    const Voxel* getVoxel(const GLint x, const GLint y, const GLint z);
    void setSeed(const GLuint seed);
    const bool test(const GLint x, const GLint y, const GLint z);
  private:
    FastNoise noise;
    Chunk* generate(const GLint cx, const GLint cz);
    std::map<std::pair<GLint, GLint>, Chunk*> cache;
    static const Voxel air;
    static const Voxel bedrock;
};

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "color.hpp"
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
    const GLuint getSeed();
    void setSeed(const GLuint seed);
  private:
    FastNoise noise;
    color::hsv<GLfloat> hsv;
    color::rgb<GLfloat> rgb;
    std::map<std::pair<GLint, GLint>, Chunk*> cache;
    Chunk* generate(const GLint cx, const GLint cz);
};

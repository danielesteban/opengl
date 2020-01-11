#include "chunks.hpp"

Chunks::Chunks() {
  noise.SetFrequency(0.005f);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

const Chunk* Chunks::get(const GLint x, const GLint z) {
  std::pair<GLint, GLint> key = std::make_pair(x, z);
  std::map<std::pair<GLint, GLint>, Chunk*>::iterator it = cache.find(key);
  Chunk *chunk;
  if (it == cache.end()) {
    chunk = generate(x, z);
    cache[key] = chunk;
  } else {
    chunk = it->second;
  }
  return chunk;
}

Chunk* Chunks::generate(const GLint cx, const GLint cz) {
  Chunk *chunk = new Chunk();

  const GLint vx = cx * ChunkSize;
  const GLint vz = cz * ChunkSize;
  for (GLint i = 0, z = 0; z < ChunkSize; z++) {
    for (GLint x = 0; x < ChunkSize; x++, i++) {
      chunk->heightmap[i] = (GLubyte) (
        fmax(noise.GetNoise(
          (GLfloat) (vx + x),
          (GLfloat) (vz + z)
        ) + 0.5f, 0.0f) * ChunkSize * NumSubchunks
      );
    }
  }

  for (GLint i = 0, z = 0; z < ChunkSize; z++) {
    for (GLint y = 0; y < ChunkSize * NumSubchunks; y++) {
      for (GLint x = 0; x < ChunkSize; x++, i++) {
        if (
          y == 0
          || (
            y <= chunk->heightmap[z * ChunkSize + x]
            && noise.GetPerlin(
              (GLfloat) (vx + x) * 4.0f,
              (GLfloat) y * 2.0f,
              (GLfloat) (vz + z) * 4.0f
            ) >= -0.2f
          )
        ) {
          chunk->voxels[i].type = 1;
          hsv[0] = fmax(noise.GetPerlin(
            (GLfloat) (vz + z),
            (GLfloat) y,
            (GLfloat) (vx + x)
          ) + 0.5f, 0.0f) * 360.0f;
          hsv[1] = (1.0f - fmin((GLfloat) y / 50.0f, 1.0f)) * 100.0f;
          hsv[2] = (GLfloat) (rand() % 60 + 20);
          rgb = hsv;
          chunk->voxels[i].color = glm::vec3(rgb[0], rgb[1], rgb[2]);
        }
      }
    }
  }

  return chunk;
}

void Chunks::setSeed(const GLuint seed) {
  cache.clear();
  noise.SetSeed(seed);
}

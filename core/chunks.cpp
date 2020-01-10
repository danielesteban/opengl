#include "chunks.hpp"

const Voxel Chunks::air = {0, { 0.0f, 0.0f, 0.0f }};
const Voxel Chunks::bedrock = {0xFF, { 0.0f, 0.0f, 0.0f }};

Chunks::Chunks() {
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

const Voxel* Chunks::getVoxel(const GLint x, const GLint y, const GLint z) {
  if (y < 0) {
    return &bedrock;
  }
  if (y >= ChunkSize * NumSubchunks) {
    return &air;
  }
  const GLint cx = (GLint) floor((GLfloat) x / ChunkSize);
  const GLint cz = (GLint) floor((GLfloat) z / ChunkSize);
  const Chunk *chunk = get(cx, cz);
  const GLint vx = x - (cx * ChunkSize);
  const GLint vz = z - (cz * ChunkSize);
  const GLint i = (vz * ChunkSize * ChunkSize * NumSubchunks) + (y * ChunkSize) + vx;
  return &chunk->voxels[i];
}

const bool Chunks::test(const GLint x, const GLint y, const GLint z) {
  const Voxel *voxel = getVoxel(x, y, z);
  return voxel->type != 0;
}

void Chunks::setSeed(const GLuint seed) {
  noise.SetSeed(seed);
}

Chunk* Chunks::generate(const GLint cx, const GLint cz) {
  Chunk *chunk = new Chunk();

  const GLint vx = cx * ChunkSize;
  const GLint vz = cz * ChunkSize;
  for (GLint i = 0, z = 0; z < ChunkSize; z++) {
    for (GLint x = 0; x < ChunkSize; x++, i++) {
      chunk->heightmap[i] = (GLubyte) (
        abs(noise.GetNoise(
          (GLfloat) (vx + x) / 1.5f,
          (GLfloat) (vz + z) / 1.5f
        ) + 0.5f) * ChunkSize * NumSubchunks
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
            && noise.GetNoise(
              (GLfloat) (vz + z),
              (GLfloat) y * 0.5f,
              (GLfloat) (vx + x)
            ) >= 0.0f
          )
        ) {
          chunk->voxels[i].type = 1;
          hsv[0] = fmax(noise.GetNoise(
            (GLfloat) (vz + z) / 1.5f,
            (GLfloat) y * 1.5f,
            (GLfloat) (vx + x) / 1.5f
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

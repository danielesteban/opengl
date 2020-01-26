#pragma once

#include "scene.hpp"
#include "chunks.hpp"

class Gameplay : public Scene {
  public:
    Gameplay(Camera &camera);
    void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta);
    void debug();
  private:
    Chunks chunks;
    typedef struct {
      Mesh *meshes[NumSubchunks];
    } VoxelsChunk;
    std::map<std::pair<GLint, GLint>, VoxelsChunk> loaded;
    std::vector<std::pair<GLint, GLint>> queued;
    typedef struct {
      GLint x, z;
    } ChunkPosition;
    ChunkPosition playerChunk;
    std::vector<ChunkPosition> renderGrid;
    static const GLint renderRadius;
    Shader *voxelsShader;
    Texture *noiseTexture;
    void reset();
};

#pragma once

#include "scene.hpp"
#include "chunks.hpp"

class VoxelsScene : public Scene {
  public:
    VoxelsScene();
    void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta);
    void debug();
  private:
    Chunks chunks;
    typedef struct {
      struct {
        GLint x, y, z;
      } chunk;
      Mesh *mesh;
    } Voxels;
    std::vector<Voxels> voxels;
    GLint generationTime;
    void generate();
    static const GLint renderRadius; 
};

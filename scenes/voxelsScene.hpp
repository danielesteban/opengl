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
    std::vector<Mesh *> voxels;
    GLint generationTime;
    void generate();
    static const GLint renderRadius; 
};

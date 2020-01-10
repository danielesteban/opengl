#pragma once

#include "chunks.hpp"
#include "scene.hpp"

class VoxelsScene : public Scene {
  public:
    VoxelsScene();
    void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta);
  private:
    Chunks chunks;
};
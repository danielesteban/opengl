#pragma once

#include "chunks.hpp"
#include "scene.hpp"

class VoxelsScene : public Scene {
  public:
    VoxelsScene();
  private:
    Chunks chunks;
};

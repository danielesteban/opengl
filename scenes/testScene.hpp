#pragma once

#include "FastNoise.h"
#include "scene.hpp"

class TestScene : public Scene {
  public:
    TestScene();
    void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta);
  private:
    std::vector<Mesh *> cubes;
    FastNoise noise;
    void generate();
};

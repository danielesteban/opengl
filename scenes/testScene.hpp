#pragma once

#include <FastNoise.h>
#include "scene.hpp"
#include "boxGeometry.hpp"
#include "planeGeometry.hpp"
#include "basicShader.hpp"
#include "gridShader.hpp"

class TestScene : public Scene {
  public:
    TestScene();
    void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta);
  private:
    BasicShader basicShader;
    GridShader gridShader;
    BoxGeometry box;
    PlaneGeometry plane;
    std::vector<Mesh *> cubes;
    FastNoise noise;
    void generate();
};

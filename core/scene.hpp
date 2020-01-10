#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.hpp"
#include "input.hpp"
#include "geometry.hpp"
#include "mesh.hpp"
#include "shader.hpp"

class Scene {
  public:
    virtual ~Scene() {
      for (auto *geometry : geometries) {
        delete geometry;
      }
      geometries.clear();
      for (auto *shader : shaders) {
        delete shader;
      }
      shaders.clear();
    }
    virtual void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta) {}
    std::vector<Geometry *> geometries;
    std::vector<Mesh *> meshes;
    std::vector<Mesh *> transparentMeshes;
    std::vector<Shader *> shaders;
};

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "shader.hpp"

class Scene {
  public:
    virtual void animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta) = 0;
    std::vector<Mesh *> meshes;
    std::vector<Shader *> shaders;
};

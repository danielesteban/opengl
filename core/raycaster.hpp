#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "input.hpp"

class Raycaster {
  public:
    Raycaster();
    struct {
      glm::vec3 origin;
      glm::vec3 direction;
    } ray;
    bool intersectsSphere(const glm::vec3 &origin, const GLfloat radius);
    void setFromCamera(const Camera &camera, const Input &input);
};

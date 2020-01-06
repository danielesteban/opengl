#pragma once

#include <glm/gtc/quaternion.hpp>
#include "camera.hpp"
#include "geometry.hpp"
#include "shader.hpp"

class Mesh {
  public:
    Mesh(Geometry *geometry, Shader *shader);
    glm::vec3 albedo;
    struct {
      glm::vec3 origin;
      GLfloat radius;
    } culling;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    void render();
    void updateTransform();
  private:
    Geometry *geometry;
    Shader *shader;
    glm::mat4 transform;
};

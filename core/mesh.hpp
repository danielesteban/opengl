#pragma once

#include <glm/gtc/quaternion.hpp>
#include "camera.hpp"
#include "geometry.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Mesh {
  public:
    Mesh(
      Geometry *geometry,
      Shader *shader,
      Texture *texture = nullptr,
      glm::vec3 position = glm::vec3(),
      glm::quat rotation = glm::quat(),
      glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
    );
    Mesh(
      Geometry *geometry,
      Shader *shader,
      glm::vec3 position = glm::vec3(),
      glm::quat rotation = glm::quat(),
      glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
    );
    Geometry *geometry;
    Shader *shader;
    Texture *texture;
    glm::vec3 albedo;
    struct {
      glm::vec3 origin;
      GLfloat radius;
    } culling;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::mat4 transform;
    bool visible;
    void render();
    void updateTransform();    
};

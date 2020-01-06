#include "mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

Mesh::Mesh(Geometry *geometry, Shader *shader) :
  albedo({ 1.0f, 1.0f, 1.0f }),
  geometry(geometry),
  shader(shader),
  scale({ 1.0f, 1.0f, 1.0f })
{
  updateTransform();
}

void Mesh::render() {
  shader->use();
  shader->updateAlbedo(glm::value_ptr(albedo));
  shader->updateModel(glm::value_ptr(transform));
  geometry->draw();
}

void Mesh::updateTransform() {
  transform = glm::translate(glm::mat4(1.0f), position);
  transform *= glm::toMat4(rotation);
  transform = glm::scale(transform, scale);
  culling.origin = transform * glm::vec4(geometry->origin, 1.0f);
  culling.radius = geometry->radius * fmax(scale.x, fmax(scale.y, scale.z));
}

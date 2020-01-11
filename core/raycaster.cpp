#include "raycaster.hpp"

Raycaster::Raycaster() {

}

bool Raycaster::intersectsSphere(const glm::vec3 &origin, const GLfloat radius) {
  GLfloat distance;
  GLfloat directionDistance = glm::dot(origin - ray.origin, ray.direction);
  if (directionDistance < 0) {
    distance = glm::distance(ray.origin, origin);
  } else {
    distance = glm::distance(
      (ray.direction * directionDistance) + ray.origin,
      origin
    );
  }
  return distance <= radius;
}

void Raycaster::setFromCamera(const Camera &camera, const Input &input) {
  ray.origin = camera.position;
  ray.direction = camera.front;
}

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLfloat Camera::fov = glm::radians(75.0f);
const GLfloat Camera::farPlane = 1000.0f;
const GLfloat Camera::maxPitch = glm::radians(89.0f);
const GLfloat Camera::nearPlane = 0.1f;
const GLfloat Camera::sensitivity = 0.002f;
const GLfloat Camera::speed = 10.0f;
const glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() :
  position(0.0f, 2.5f, 0.0f),
  yaw(glm::radians(270.0f))
{
  updateVectors();
}

void Camera::animate(const Input &input, const GLfloat time, const GLfloat delta) {
  if (input.mouse.movement.x != 0 || input.mouse.movement.y != 0) {
    yaw += input.mouse.movement.x * sensitivity;
    pitch += input.mouse.movement.y * sensitivity;
    updateVectors();
  }

  if (
    input.keyboard.forwards
    || input.keyboard.backwards
    || input.keyboard.leftwards
    || input.keyboard.rightwards
    || input.keyboard.upwards
    || input.keyboard.downwards
  ) {
    glm::vec3 movement;
    if (input.keyboard.forwards) {
      movement += front;
    }
    if (input.keyboard.backwards) {
      movement -= front;
    }
    if (input.keyboard.leftwards) {
      movement -= right;
    }
    if (input.keyboard.rightwards) {
      movement += right;
    }
    if (input.keyboard.upwards) {
      movement += worldUp;
    }
    if (input.keyboard.downwards) {
      movement -= worldUp;
    }
    if (glm::length(movement) > 0) {
      position += glm::normalize(movement) * delta * speed * (input.keyboard.downwards ? 2.0f : 1.0f);
      updateView();
    }
  }
}

bool Camera::isInFrustum(const glm::vec3 &origin, const GLfloat radius) {
  for (int i = 0; i < 6; i++) {
    const GLfloat distance = glm::dot(frustum[i].normal, origin) + frustum[i].constant;
    if (distance < -radius) {
      return false;
    }
  }
  return true;
}

void Camera::resize(const GLint width, const GLint height) {
  const GLfloat aspect = (GLfloat) width / (GLfloat) height;
  projection = glm::perspective(fov, aspect, nearPlane, farPlane);
  updateFrustum();
}

void Camera::updateFrustum() {
  transform = projection * view;
  GLfloat* m = glm::value_ptr(transform);
  updateFrustumPlane(0, m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
  updateFrustumPlane(1, m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
  updateFrustumPlane(2, m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);
  updateFrustumPlane(3, m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);
  updateFrustumPlane(4, m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
  updateFrustumPlane(5, m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
}

void Camera::updateFrustumPlane(const GLuint index, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w) {
  frustum[index].normal = glm::vec3(x, y, z);
  const GLfloat inverseNormalLength = 1.0f / glm::length(frustum[index].normal);
  frustum[index].normal = glm::normalize(frustum[index].normal * inverseNormalLength);
  frustum[index].constant = w * inverseNormalLength;
}

void Camera::updateVectors() {
  pitch = fmin(fmax(pitch, -maxPitch), maxPitch);
  front.x = cos(yaw) * cos(pitch);
  front.y = sin(pitch);
  front.z = sin(yaw) * cos(pitch);
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  updateView();
}

void Camera::updateView() {
  view = glm::lookAt(
    position,
    position + front,
    worldUp
  );
  updateFrustum();
}

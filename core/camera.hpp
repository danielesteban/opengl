#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "input.hpp"

class Camera {
  public:
    Camera();
    glm::vec3 front;
    GLfloat pitch;
    glm::vec3 position;
    glm::mat4 projection;
    glm::vec3 right;
    glm::mat4 view;
    GLfloat yaw;
    void animate(const Input &input, const GLfloat time, const GLfloat delta);
    bool isInFrustum(const glm::vec3 &origin, const GLfloat radius);
    void resize(const GLint width, const GLint height);
    void updateVectors();
    void updateView();
    static const glm::vec3 worldUp;
  private:
    struct {
      glm::vec3 normal;
      GLfloat constant;
    } frustum[6];
    glm::mat4 transform;
    static const GLfloat farPlane;
    static const GLfloat fov;
    static const GLfloat maxPitch;
    static const GLfloat nearPlane;
    static const GLfloat sensitivity;
    static const GLfloat speed;
    void updateFrustum();
    void updateFrustumPlane(const GLuint index, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w);
};

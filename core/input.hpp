#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input {
  public:
    static Input& getInstance() {
      static Input instance;
      return instance;
    }

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
      getInstance().onMouseMove(window, xpos, ypos); 
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      getInstance().onKeyboard(window, key, scancode, action, mods); 
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
      getInstance().onMouseButton(window, button, action, mods); 
    }

    static void windowFocusCallback(GLFWwindow* window, int focused) {
      getInstance().onFocus(window, focused); 
    }

    bool isLocked;
    struct {
      bool backwards, downwards, forwards, leftwards, rightwards, upwards;
    } keyboard;
    struct {
      bool primary, primaryDown, secondary, secondaryDown;
      glm::vec2 position, movement;
    } mouse;

    void onFocus(GLFWwindow* window, int focused) {
      if (isLocked && focused == GLFW_FALSE) {
        unlockInput(window);
      }
    }
    
    void onKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
      if (action != GLFW_PRESS && action != GLFW_RELEASE) {
        return;
      }
      const bool hasPressed = action == GLFW_PRESS;
      switch (key) {
        case GLFW_KEY_ESCAPE:
          if (hasPressed && isLocked) {
            unlockInput(window);
          }
          break;
        case GLFW_KEY_W:
          keyboard.forwards = hasPressed;
          break;
        case GLFW_KEY_S:
          keyboard.backwards = hasPressed;
          break;
        case GLFW_KEY_A:
          keyboard.leftwards = hasPressed;
          break;
        case GLFW_KEY_D:
          keyboard.rightwards = hasPressed;
          break;
        case GLFW_KEY_LEFT_SHIFT:
          keyboard.downwards = hasPressed;
          break;
        case GLFW_KEY_SPACE:
          keyboard.upwards = hasPressed;
          break;
        default:
          break;
      }
    }

    void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
      if (action != GLFW_PRESS && action != GLFW_RELEASE) {
        return;
      }
      const bool hasPressed = action == GLFW_PRESS;
      if (isLocked) {
        switch (button) {
          case GLFW_MOUSE_BUTTON_1:
            mouse.primary = hasPressed;
            mouse.primaryDown = hasPressed;
            break;
          case GLFW_MOUSE_BUTTON_2:
            mouse.secondary = hasPressed;
            mouse.secondaryDown = hasPressed;
            break;
          default:
            break;
        }
      } else if (hasPressed) {
        lockInput(window);
      }
    }

    void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
      if (isLocked) {
        mouse.movement.x += (GLfloat) xpos - mouse.position.x;
        mouse.movement.y += mouse.position.y - (GLfloat) ypos;
      }
      mouse.position.x = (GLfloat) xpos;
      mouse.position.y = (GLfloat) ypos;
    }
  private:
    Input(void) {}
    Input(Input const&);
    void operator=(Input const&);

    void lockInput(GLFWwindow *window) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      isLocked = true;
    }

    void unlockInput(GLFWwindow *window) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      isLocked = false;
      mouse.primary = false;
      mouse.primaryDown = false;
      mouse.secondary = false;
      mouse.secondaryDown = false;
      keyboard.backwards = false;
      keyboard.downwards = false;
      keyboard.forwards = false;
      keyboard.leftwards = false;
      keyboard.rightwards = false;
      keyboard.upwards = false;
    }
};

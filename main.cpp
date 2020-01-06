#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "camera.hpp"
#include "input.hpp"
#include "testScene.hpp"

const GLint windowSize[2] = { 1280, 720 };

bool needsResize = true;
void onResize(GLFWwindow* window, int width, int height) {
  needsResize = true;
}

int main(void) {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow *window = glfwCreateWindow(windowSize[0], windowSize[1], "C++ is easy  |  dani@gatunes © 2020 ", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
    glfwSetWindowPos(
      window,
      monitorX + (mode->width - windowSize[0]) / 2,
      monitorY + (mode->height - windowSize[1]) / 2
    );
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glfwSetFramebufferSizeCallback(window, onResize);
  glfwSwapInterval(1);

  Input &input = Input::getInstance();
  glfwSetCursorPosCallback(window, &Input::cursorPosCallback);
  glfwSetKeyCallback(window, &Input::keyCallback);
  glfwSetMouseButtonCallback(window, &Input::mouseButtonCallback);
  glfwSetWindowFocusCallback(window, &Input::windowFocusCallback);

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = NULL;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
  ImGuiStyle& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

  Camera camera;
  TestScene scene;

  struct {
    GLuint acc, lastCount;
    GLfloat lastTick;
  } fps = { 0, 0, 0.0f };
  GLfloat lastFrame = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    GLfloat time = (GLfloat) glfwGetTime();
    GLfloat delta = time - lastFrame;
    lastFrame = time;
    fps.acc++;
    if (time >= fps.lastTick + 1) {
      fps.lastCount = (GLuint) round((GLfloat) fps.acc / (time - fps.lastTick));
      fps.lastTick = time;
      fps.acc = 0;
    }

    camera.animate(&input, time, delta);
    scene.animate(&camera, &input, time, delta);
    input.mouse.movement = glm::vec2(0, 0);

    for(
      std::vector<Shader *>::iterator i = scene.shaders.begin();
      i != scene.shaders.end();
      i++
    ) {
      (*i)->use();
      (*i)->updateCamera(&camera);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint inFrustum = 0;
    for (
      std::vector<Mesh *>::iterator i = scene.meshes.begin();
      i != scene.meshes.end();
      i++
    ) {
      Mesh *mesh = (*i);
      if (camera.isInFrustum(mesh->culling.origin, mesh->culling.radius)) {
        mesh->render();
        inFrustum++;
      }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 110.0f, 10.0f), 0);
    ImGui::SetNextWindowSize(ImVec2(100.0f, 0.0f), 0);
    ImGui::Begin("Debug");
    ImGui::Text("%d Meshes", scene.meshes.size());
    ImGui::Text("%d Frustum", inFrustum);
    ImGui::Text("%d Shaders", scene.shaders.size());
    ImGui::Text("%d FPS", fps.lastCount);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
    if (needsResize) {
      GLint width, height;
      glfwGetFramebufferSize(window, &width, &height);
      glViewport(0, 0, width, height);
      camera.resize(width, height);
      needsResize = false;
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

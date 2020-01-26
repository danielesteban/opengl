#include "gameplay.hpp"
#include "color.hpp"

#include "basicShader.hpp"
#include "gridShader.hpp"
#include "oceanShader.hpp"
#include "voxelsShader.hpp"

#include "noisetexture.hpp"

#include "cloudGeometry.hpp"
#include "planeGeometry.hpp"
#include "voxelsGeometry.hpp"

const GLint Gameplay::renderRadius = 8;

Gameplay::Gameplay() {
  Shader *basicShader = new BasicShader();
  shaders.push_back(basicShader);
  Shader *gridShader = new GridShader();
  shaders.push_back(gridShader);
  Shader *oceanShader = new OceanShader();
  shaders.push_back(oceanShader);
  Shader *voxelsShader = new VoxelsShader();
  shaders.push_back(voxelsShader);

  glm::vec3 background(0.4f, 0.7f, 1.0f);
  glClearColor(background.x, background.y, background.z, 1.0f);
  for (auto *shader : shaders) {
    shader->use();
    shader->updateFog(background, shader == basicShader ? 0.005f : 0.015f);
  }

  Texture *noiseTexture = new NoiseTexture();
  textures.push_back(noiseTexture);

  Geometry *plane = new PlaneGeometry(1000.0f, 1000.0f);
  geometries.push_back(plane);
  {
    Mesh *mesh = new Mesh(plane, gridShader);
    mesh->rotation = glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
    mesh->updateTransform();
    meshes.push_back(mesh);
  }

  {
    Mesh *mesh = new Mesh(plane, oceanShader);
    mesh->position = glm::vec3(0.0f, 4.5f, 0.0f);
    mesh->rotation = glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
    mesh->updateTransform();
    transparentMeshes.push_back(mesh);
  }
  
  const GLfloat cloudRadius = 512.0f;
  for (GLint z = 0; z < 3; z += 1) {
    for (GLint x = 0; x < 3; x += 1) {
      Geometry *geometry = new CloudGeometry();
      geometries.push_back(geometry);
      Mesh *mesh = new Mesh(geometry, basicShader);
      mesh->position = glm::vec3(
        -cloudRadius + ((GLfloat) x * cloudRadius),
        128.0f + (GLfloat) (rand() % 128),
        -cloudRadius + ((GLfloat) z * cloudRadius)
      );
      mesh->updateTransform();
      meshes.push_back(mesh);
    }
  }

  for (GLint z = -renderRadius; z <= renderRadius; z++) {
    for (GLint y = 0; y < NumSubchunks; y++) {
      for (GLint x = -renderRadius; x <= renderRadius; x++) {
        if (sqrt(pow((GLfloat) z + 0.5f, 2) + pow((GLfloat) x + 0.5f, 2)) >= renderRadius) {
          continue;
        }
        Geometry *geometry = new VoxelsGeometry();
        geometries.push_back(geometry);
        Mesh *mesh = new Mesh(geometry, voxelsShader, noiseTexture);
        mesh->position = glm::vec3(
          ChunkSize * -0.5f + (GLfloat) x * ChunkSize,
          (GLfloat) y * ChunkSize,
          ChunkSize * -0.5f + (GLfloat) z * ChunkSize
        );
        mesh->updateTransform();
        mesh->visible = false;
        meshes.push_back(mesh);
        voxels.push_back({ { x, y, z }, mesh });
      }
    }
  }

  generate();
}

void Gameplay::animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta) {
  if (camera.position.y < 5.0f) {
    camera.position.y = 5.0f;
    camera.updateView();
  }
}

void Gameplay::debug() {
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  
  ImGui::Text("World seed: %d", chunks.getSeed());
  ImGui::Text("%d chunks in %dms", voxels.size() / NumSubchunks, generationTime);

  ImGui::Spacing();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
  if (ImGui::Button("regenerate")) {
    generate();
  }
  ImGui::Spacing();
}

void Gameplay::generate() {
  const GLfloat start = (GLfloat) glfwGetTime();
  chunks.setSeed(rand());
  for (auto v : voxels) {
    ((VoxelsGeometry *) v.mesh->geometry)->generate(&chunks, v.chunk.x, v.chunk.y, v.chunk.z);
    v.mesh->visible = v.mesh->geometry->count != 0;
  }
  generationTime = (GLint) (((GLfloat) glfwGetTime() - start) * 1000.0f);
}

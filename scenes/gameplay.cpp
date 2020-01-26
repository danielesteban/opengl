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

Gameplay::Gameplay(Camera &camera) {
  Shader *basicShader = new BasicShader();
  shaders.push_back(basicShader);
  Shader *gridShader = new GridShader();
  shaders.push_back(gridShader);
  Shader *oceanShader = new OceanShader();
  shaders.push_back(oceanShader);
  voxelsShader = new VoxelsShader();
  shaders.push_back(voxelsShader);

  glm::vec3 background(0.4f, 0.7f, 1.0f);
  glClearColor(background.x, background.y, background.z, 1.0f);
  for (auto *shader : shaders) {
    shader->use();
    shader->updateFog(background, shader == basicShader ? 0.005f : 0.015f);
  }

  noiseTexture = new NoiseTexture();
  textures.push_back(noiseTexture);

  Geometry *plane = new PlaneGeometry(1000.0f, 1000.0f);
  geometries.push_back(plane);
  {
    meshes.push_back(new Mesh(
      plane,
      gridShader,
      glm::vec3(),
      glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f))
    ));
  }

  {
    transparentMeshes.push_back(new Mesh(
      plane,
      oceanShader,
      glm::vec3(0.0f, 4.5f, 0.0f),
      glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f))
    ));
  }
  
  const GLfloat cloudRadius = 512.0f;
  for (GLint z = 0; z < 3; z += 1) {
    for (GLint x = 0; x < 3; x += 1) {
      Geometry *geometry = new CloudGeometry();
      geometries.push_back(geometry);
      meshes.push_back(new Mesh(
        geometry,
        basicShader,
        glm::vec3(
          -cloudRadius + ((GLfloat) x * cloudRadius),
          128.0f + (GLfloat) (rand() % 128),
          -cloudRadius + ((GLfloat) z * cloudRadius)
        )
      ));
    }
  }

  for (GLint z = -renderRadius; z <= renderRadius; z++) {
    for (GLint x = -renderRadius; x <= renderRadius; x++) {
      if (
        sqrt(pow((GLfloat) x, 2) + pow((GLfloat) z, 2)) <= renderRadius
      ) {
        renderGrid.push_back({ x, z });
      }
    }
  }
  std::sort(renderGrid.begin(), renderGrid.end(), [](ChunkPosition a, ChunkPosition b) {
    return (
      sqrt(pow((GLfloat) a.x, 2) + pow((GLfloat) a.z, 2))
      < sqrt(pow((GLfloat) b.x, 2) + pow((GLfloat) b.z, 2))
    );
  });

  camera.position.x = ChunkSize * 0.5f + 0.5f;
  camera.position.z = ChunkSize * 0.5f + 0.5f;
  camera.updateView();

  reset();
}

void Gameplay::animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta) {
  if (camera.position.y < 5.0f) {
    camera.position.y = 5.0f;
    camera.updateView();
  }

  const GLint cameraChunkX = (GLint) floor(camera.position.x / ChunkSize);
  const GLint cameraChunkZ = (GLint) floor(camera.position.z / ChunkSize);
  if (
    playerChunk.x != cameraChunkX
    || playerChunk.z != cameraChunkZ
  ) {
    playerChunk.x = cameraChunkX;
    playerChunk.z = cameraChunkZ;

    queued.clear();
    for (auto offset : renderGrid) {
      std::pair<GLint, GLint> chunk = std::make_pair(playerChunk.x + offset.x, playerChunk.z + offset.z);
      if (loaded.find(chunk) == loaded.end()) {
        queued.push_back(chunk);
      }
    }

    std::map<std::pair<GLint, GLint>, VoxelsChunk>::iterator chunk = loaded.begin();
    while (chunk != loaded.end()) {
      if (
        sqrt(pow((GLfloat) (chunk->first.first - playerChunk.x), 2) + pow((GLfloat) (chunk->first.second - playerChunk.z), 2)) > renderRadius
      ) {
        for (GLint y = 0; y < NumSubchunks; y++) {
          Mesh *mesh = chunk->second.meshes[y];
          geometries.erase(std::find(geometries.begin(), geometries.end(), mesh->geometry));
          meshes.erase(std::find(meshes.begin(), meshes.end(), mesh));
          delete mesh->geometry;
        }
        chunk = loaded.erase(chunk);
      } else {
        chunk++;
      }
    }
  }

  if (!queued.empty()) {
    std::vector<std::pair<GLint, GLint>>::iterator queuedChunk = queued.begin();
    std::pair<GLint, GLint> chunk = (*queuedChunk);
    queued.erase(queuedChunk);

    VoxelsChunk voxelsChunk;
    for (GLint y = 0; y < NumSubchunks; y++) {
      VoxelsGeometry *geometry = new VoxelsGeometry();
      geometry->generate(&chunks, chunk.first, y, chunk.second);
      geometries.push_back(geometry);
      Mesh *mesh = new Mesh(
        geometry,
        voxelsShader,
        noiseTexture,
        glm::vec3(
          (GLfloat) chunk.first * ChunkSize,
          (GLfloat) y * ChunkSize,
          (GLfloat) chunk.second * ChunkSize
        )
      );
      mesh->visible = geometry->count != 0;
      meshes.push_back(mesh);
      voxelsChunk.meshes[y] = mesh;
    }
    if (loaded.size() == 0) {
      const Chunk *data = chunks.get(chunk.first, chunk.second);
      const GLint cameraX = (GLint) floor(camera.position.x - (cameraChunkX * ChunkSize));
      const GLint cameraZ = (GLint) floor(camera.position.z - (cameraChunkZ * ChunkSize));
      camera.position.y = data->heightmap[cameraZ * ChunkSize + cameraX] + 1.5f;
      camera.updateView();
    }
    loaded[chunk] = voxelsChunk;
  }
}

void Gameplay::debug() {
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  
  ImGui::Text("WorldSeed: %d", chunks.getSeed());
  ImGui::Spacing();
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 32.0f);
  if (ImGui::Button("regenerate", ImVec2(100.0f, 20.f))) {
    reset();
  }
  ImGui::Spacing();
}

void Gameplay::reset() {
  chunks.setSeed(rand());
  queued.clear();
  for (std::map<std::pair<GLint, GLint>, VoxelsChunk>::iterator chunk = loaded.begin(); chunk != loaded.end(); chunk++) {
    for (GLint y = 0; y < NumSubchunks; y++) {
      Mesh *mesh = chunk->second.meshes[y];
      geometries.erase(std::find(geometries.begin(), geometries.end(), mesh->geometry));
      meshes.erase(std::find(meshes.begin(), meshes.end(), mesh));
      delete mesh->geometry;
    }
  }
  loaded.clear();
  playerChunk.x -= 1;
  playerChunk.z -= 1;
}

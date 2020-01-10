#include "voxelsScene.hpp"
#include "color.hpp"

#include "gridShader.hpp"
#include "oceanShader.hpp"
#include "voxelsShader.hpp"

#include "noisetexture.hpp"

#include "planeGeometry.hpp"
#include "voxelsGeometry.hpp"

VoxelsScene::VoxelsScene() {
  GridShader *gridShader = new GridShader();
  shaders.push_back(gridShader);
  OceanShader *oceanShader = new OceanShader();
  shaders.push_back(oceanShader);
  VoxelsShader *voxelsShader = new VoxelsShader();
  shaders.push_back(voxelsShader);

  Texture *noiseTexture = (Texture *) new NoiseTexture();
  textures.push_back(noiseTexture);

  Geometry *plane = (Geometry *) new PlaneGeometry(1000.0f, 1000.0f);
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

  chunks.setSeed(rand());
  const GLint renderRadius = 8;
  for (GLint z = -renderRadius; z <= renderRadius; z++) {
    for (GLint x = -renderRadius; x <= renderRadius; x++) {
      for (GLint y = 0; y < NumSubchunks; y++) {
        Geometry *geometry = (Geometry *) new VoxelsGeometry(&chunks, x, y, z);
        if (geometry->count == 0) {
          delete geometry;
          geometry = nullptr;
          continue;
        }
        geometries.push_back(geometry);
        Mesh *mesh = new Mesh(geometry, voxelsShader, noiseTexture);
        mesh->position = glm::vec3(
          ChunkSize * -0.5f + (GLfloat) x * ChunkSize,
          (GLfloat) y * ChunkSize,
          ChunkSize * -0.5f + (GLfloat) z * ChunkSize
        );
        mesh->updateTransform();
        meshes.push_back(mesh);
      }
    }
  }
}

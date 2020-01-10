#include "testScene.hpp"
#include "color.hpp"

#include "basicShader.hpp"
#include "gridShader.hpp"

#include "boxGeometry.hpp"
#include "planeGeometry.hpp"


TestScene::TestScene() {
  BasicShader *basicShader = new BasicShader();
  shaders.push_back(basicShader);
  GridShader *gridShader = new GridShader();
  shaders.push_back(gridShader);

  {
    Geometry *geometry = (Geometry *) new PlaneGeometry(1000.0f, 1000.0f);
    geometries.push_back(geometry);
    Mesh *mesh = new Mesh(geometry, gridShader);
    mesh->rotation = glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));
    mesh->updateTransform();
    meshes.push_back(mesh);
  }

  noise.SetNoiseType(FastNoise::SimplexFractal);

  Geometry *box = (Geometry *) new BoxGeometry(1.0f, 1.0f, 1.0f);
  geometries.push_back(box);
  for (int z = 0; z < 100; z += 1) {
    for (int x = 0; x < 100; x += 1) {
      Mesh *cube = new Mesh(box, basicShader);
      cube->position.x = (GLfloat) (x - 50) * 1.5f;
      cube->position.z = (GLfloat) (z - 50) * 1.5f;
      cubes.push_back(cube);
      meshes.push_back(cube);
    }
  }

  generate();
}

void TestScene::animate(Camera &camera, const Input &input, const GLfloat time, const GLfloat delta) {
  for (auto *cube : cubes) {
    cube->rotation *= glm::quat(glm::vec3(
      0.0f,
      delta * (cube->position.x + cube->position.z) * 0.1f,
      0.0f
    ));
    cube->updateTransform();
  }
  if (input.mouse.primaryDown) {
    generate();
    int x = (int) floor(fmin(fmax(camera.position.x / 1.5f + 50.0f, 0.0f), 99.0f));
    int z = (int) floor(fmin(fmax(camera.position.z / 1.5f + 50.0f, 0.0f), 99.0f));
    Mesh *cube = cubes[z * 100 + x];
    camera.position.y = cube->position.y + 2.0f;
    camera.updateView();
  }
}

void TestScene::generate() {
  noise.SetSeed(rand());
  glm::vec2 offset(rand(), rand());
  color::hsv<GLfloat> hsv;
  color::rgb<GLfloat> rgb;
  for (auto *cube : cubes) {
    cube->position.y = fmax(
      noise.GetNoise(
        offset.x + cube->position.x / 1.5f,
        offset.y + cube->position.z / 1.5f
      ) + 0.5f,
      0.0f
    ) * 50.0f + 0.5f;
    cube->updateTransform();
    hsv[0] = (GLfloat) (rand() % 360);
    hsv[1] = (1.0f - fmin(cube->position.y / 50.0f, 1.0f)) * 100.0f;
    hsv[2] = (GLfloat) (rand() % 60 + 20);
    rgb = hsv; 
    cube->albedo = glm::vec3(rgb[0], rgb[1], rgb[2]);
  }
}

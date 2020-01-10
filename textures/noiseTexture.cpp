#include "noiseTexture.hpp"
#include <random>

NoiseTexture::NoiseTexture() {
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  const GLint width = 16;
  const GLint height = 16;
  GLubyte pixels[width * height * 3];
  for (GLubyte y = 0; y < 16; y++) {
    for (GLubyte x = 0; x < 16; x++) {
      GLfloat light = 0.9f + ((GLfloat) rand() / RAND_MAX) * 0.05f;
      if (
        x == 0
        || x == width - 1
        || y == 0
        || y == height - 1
      ) {
        light *= 0.9f;
      } else if (
        x == 1
        || x == width - 2
        || y == 1
        || y == height - 2
      ) {
        light *= 1.2f;
      }
      light = floor(fmin(fmax(light, 0.0f), 1.0f) * 255.0f);
      const GLint i = (y * 16 + x) * 3;
      pixels[i] = (GLubyte) light;
      pixels[i + 1] = (GLubyte) light;
      pixels[i + 2] = (GLubyte) light;
    }
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
}

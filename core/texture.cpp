#include "texture.hpp"

Texture::Texture() {
  glGenTextures(1, &texture);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture);
}

void Texture::bind() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
}

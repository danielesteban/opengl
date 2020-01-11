#include "framebuffer.hpp"
#ifndef NDEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

Framebuffer::Framebuffer(Shader *shader, const GLint samples) :
  shader(shader),
  samples(samples),
  screen(2.0f, 2.0f)
{
  glGenFramebuffers(1, &fbo);
  glGenRenderbuffers(1, &rbo);
  glGenTextures(1, &texture);
  shader->use();
  shader->updateSamples(samples);
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo);
  glDeleteRenderbuffers(1, &rbo);
  glDeleteTextures(1, &texture);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::render() {
  shader->use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
  screen.draw();
}

void Framebuffer::resize(GLint width, GLint height) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
 
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    #ifndef NDEBUG
    fprintf(stderr, "Framebuffer is not complete\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
    #endif
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  shader->use();
  shader->updateResolution(glm::vec2(width, height));
}

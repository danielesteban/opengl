#include "framebuffer.hpp"
#ifndef NDEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

Framebuffer::Framebuffer(const bool depth, const GLint samples) :
  depth(depth),
  samples(samples)
{
  glGenFramebuffers(1, &fbo);
  glGenTextures(1, &colorTexture);
  if (depth) {
    glGenTextures(1, &depthTexture);
  }
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo);
  glDeleteTextures(1, &colorTexture);
  if (depth) {
    glDeleteTextures(1, &depthTexture);
  }
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::bindColorTexture() {
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTexture);
}

void Framebuffer::bindDepthTexture() {
  if (depth) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTexture);
  }
}

void Framebuffer::resize(GLint width, GLint height) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTexture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTexture, 0);

  if (depth) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthTexture, 0);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    #ifndef NDEBUG
    fprintf(stderr, "Framebuffer is not complete\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
    #endif
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

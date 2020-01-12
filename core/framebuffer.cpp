#include "framebuffer.hpp"
#include <stdlib.h>
#ifndef NDEBUG
#include <stdio.h>
#endif

Framebuffer::Framebuffer(const bool depth, const bool multisampled, const GLint samples) :
  depth(depth),
  multisampled(multisampled),
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
  if (multisampled) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTexture);
  } else {
    glBindTexture(GL_TEXTURE_2D, colorTexture);
  }
}

void Framebuffer::bindDepthTexture() {
  if (depth) {
    if (multisampled) {
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTexture);
    } else {
      glBindTexture(GL_TEXTURE_2D, depthTexture);
    }
  }
}

void Framebuffer::bindDraw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void Framebuffer::bindRead() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void Framebuffer::resize(GLint width, GLint height) {
  this->width = width;
  this->height = height;
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  if (multisampled) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTexture, 0);

    if (depth) {
      glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthTexture);
      glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT, width, height, GL_TRUE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthTexture, 0);
    }
  } else {
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    if (depth) {
      glBindTexture(GL_TEXTURE_2D, depthTexture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    }
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    #ifndef NDEBUG
    fprintf(stderr, "Framebuffer is not complete\n");
    #endif
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

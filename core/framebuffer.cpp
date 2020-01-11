#include "framebuffer.hpp"
#ifndef NDEBUG
#include <stdlib.h>
#include <stdio.h>
#endif

const GLuint Framebuffer::numSamples = 4;

Framebuffer::Framebuffer(const bool depth, const bool multisampled) :
  depth(depth),
  multisampled(multisampled)
{
  glGenFramebuffers(1, &fbo);
  glGenRenderbuffers(1, &rbo);
  glGenTextures(1, &texture);
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo);
  glDeleteRenderbuffers(1, &rbo);
  glDeleteTextures(1, &texture);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::bindDraw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void Framebuffer::bindRead() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void Framebuffer::bindTexture() {
  glActiveTexture(GL_TEXTURE0);
  if (multisampled) {
    // TODO!
  } else {
    glBindTexture(GL_TEXTURE_2D, texture);
  }
}

void Framebuffer::resize(GLint width, GLint height) {
  this->width = width;
  this->height = height;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  if (multisampled) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numSamples, GL_RGB, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
  } else {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  }

  if (depth) {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    if (multisampled) {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);  
    } else {
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
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

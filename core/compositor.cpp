#include "compositor.hpp"

const GLint Compositor::samples = 4;

Compositor::Compositor() :
  colorBufferA(false, samples),
  colorBufferB(false, samples),
  renderBuffer(true, samples),
  screen(2.0f, 2.0f)
{
  blurShader.use();
  blurShader.updateSamples(samples);
  postprocessingShader.use();
  postprocessingShader.updateSamples(samples);
}

void Compositor::initFrame() {
  renderBuffer.bind();
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Compositor::renderFrame() {
  glDisable(GL_DEPTH_TEST);

  blurShader.use();
  glActiveTexture(GL_TEXTURE0);
  blurShader.updateFlag(true);
  colorBufferA.bind();
  renderBuffer.bindColorTexture();
  screen.draw();
  blurShader.updateFlag(false);
  colorBufferB.bind();
  colorBufferA.bindColorTexture();
  screen.draw();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  postprocessingShader.use();
  glActiveTexture(GL_TEXTURE0);
  renderBuffer.bindColorTexture();
  glActiveTexture(GL_TEXTURE1);
  renderBuffer.bindDepthTexture();
  glActiveTexture(GL_TEXTURE2);
  colorBufferB.bindColorTexture();
  screen.draw();
}

void Compositor::resize(GLint width, GLint height) {
  blurShader.use();
  blurShader.updateResolution(glm::vec2(width, height));
  postprocessingShader.use();
  postprocessingShader.updateResolution(glm::vec2(width, height));
  colorBufferA.resize(width, height);
  colorBufferB.resize(width, height);
  renderBuffer.resize(width, height);
}

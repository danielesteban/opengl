#include "compositor.hpp"

const GLint Compositor::samples = 4;

Compositor::Compositor() :
  colorBufferA(false, false, 0),
  colorBufferB(false, false, 0),
  renderBuffer(true, true, samples),
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

  renderBuffer.bindRead();
  colorBufferA.bindDraw();
  glBlitFramebuffer(
    0, 0, renderBuffer.width, renderBuffer.height,
    0, 0, colorBufferA.width, colorBufferA.height,
    GL_COLOR_BUFFER_BIT, GL_LINEAR
  );
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  blurShader.use();
  glActiveTexture(GL_TEXTURE0);
  blurShader.updateFlag(true);
  colorBufferB.bind();
  colorBufferA.bindColorTexture();
  screen.draw();
  blurShader.updateFlag(false);
  colorBufferA.bind();
  colorBufferB.bindColorTexture();
  screen.draw();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  postprocessingShader.use();
  glActiveTexture(GL_TEXTURE0);
  renderBuffer.bindColorTexture();
  glActiveTexture(GL_TEXTURE1);
  renderBuffer.bindDepthTexture();
  glActiveTexture(GL_TEXTURE2);
  colorBufferA.bindColorTexture();
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

#include "postprocessingShader.hpp"
#include "shaderChunks.hpp"

const char *PostprocessingShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPosition;\n"
  "layout (location = 1) in vec2 vUV;\n"
  "out vec2 uv;\n"
  "void main() {\n"
  "  uv = vUV;\n"
  "  gl_Position = vec4(vPosition, 1.0);\n"
  "}\n";

const char *PostprocessingShader::fragmentSource = 
  "#version 330\n"
  "in vec2 uv;\n"
  "out vec4 fragColor;\n"
  "uniform vec2 resolution;\n"
  "uniform sampler2D blurTexture;\n"
  "uniform sampler2DMS colorTexture;\n"
  "uniform sampler2DMS depthTexture;\n"
  "const float gridSize = 3.0;\n"
  LINEARIZEDEPTH_SHADER_CHUNK
  TEXTURE2DMS_SHADER_CHUNK
  "void main() {\n"
  "  ivec2 pixel = ivec2(uv * resolution);\n"
  "  vec3 blur = texture2D(blurTexture, uv).rgb;\n"
  "  vec3 color = texture2DMS(colorTexture, pixel).rgb;\n"
  "  float depth = linearizeDepth(texture2DMS(depthTexture, pixel).r);\n"
  "  vec2 grid = (mod(pixel, gridSize) / gridSize) - 0.5;\n"
  "  float lines = 1.0 - length(grid) * 0.5;\n"
  "  fragColor = vec4(mix(color, blur, clamp(depth / 128.0, 0.0, 1.0)) * lines, 1.0);\n"
  "}\n";

PostprocessingShader::PostprocessingShader() : Shader(vertexSource, fragmentSource) {

}

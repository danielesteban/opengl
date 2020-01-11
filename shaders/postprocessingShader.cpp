#include "postprocessingShader.hpp"

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
  "uniform sampler2D diffuse;\n"
  "uniform vec2 resolution;\n"
  "const float gridSize = 3.0;\n"
  "void main() {\n"
  "  vec2 pixel = uv * resolution;\n"
  "  vec2 grid = (mod(pixel, gridSize) / gridSize) - 0.5;\n"
  "  fragColor = vec4(texture2D(diffuse, uv).rgb * (1.0 - length(grid) * 0.5), 1.0);\n"
  "}\n";

PostprocessingShader::PostprocessingShader() : Shader(vertexSource, fragmentSource) {

}

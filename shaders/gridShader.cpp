#include "gridShader.hpp"
#include "fogShaderChunk.hpp"

const char *GridShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPos;\n"
  "out vec3 worldPosition;\n"
  "out vec4 viewPosition;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main() {\n"
  "  worldPosition = vPos;\n"
  "  viewPosition = view * model * vec4(vPos, 1.0);\n"
  "  gl_Position = projection * viewPosition;\n"
  "}\n";

const char *GridShader::fragmentSource = 
  "#version 330\n"
  "in vec3 worldPosition;\n"
  "in vec4 viewPosition;\n"
  "out vec4 fragColor;\n"
  FOG_SHADER_CHUNK
  "void main() {\n"
  "  vec2 coord = worldPosition.xy;\n"
  "  vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);\n"
  "  float line = min(grid.x, grid.y);\n"
  "  fragColor = vec4(fog(vec3(1.0 - min(line, 1.0)) * 0.5), 1.0);\n"
  "}\n";

GridShader::GridShader() : Shader(vertexSource, fragmentSource) {

}

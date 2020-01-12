#include "oceanShader.hpp"
#include "shaderChunks.hpp"

const char *OceanShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPos;\n"
  "out vec4 viewPosition;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main() {\n"
  "  viewPosition = view * model * vec4(vPos, 1.0);\n"
  "  gl_Position = projection * viewPosition;\n"
  "}\n";

const char *OceanShader::fragmentSource = 
  "#version 330\n"
  "in vec4 viewPosition;\n"
  "out vec4 fragColor;\n"
  "const vec3 waterColor = vec3(0.0, 0.2, 0.4);\n"
  FOG_SHADER_CHUNK
  "void main() {\n"
  "  float fogDepth = length(viewPosition);\n"
  "  float fogFactor = 1.0 - exp(-fogDensity * fogDensity * fogDepth * fogDepth);\n"
  "  fragColor = vec4(fog(waterColor), 0.8);\n"
  "}\n";

OceanShader::OceanShader() : Shader(vertexSource, fragmentSource) {

}

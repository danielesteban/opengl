#include "basicShader.hpp"
#include "shaderChunks.hpp"

const char *BasicShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPos;\n"
  "layout (location = 1) in vec3 vCol;\n"
  "out vec3 color;\n"
  "out vec3 worldPosition;\n"
  "out vec4 viewPosition;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main() {\n"
  "  color = vCol;\n"
  "  worldPosition = vPos;\n"
  "  viewPosition = view * model * vec4(vPos, 1.0);\n"
  "  gl_Position = projection * viewPosition;\n"
  "}\n";

const char *BasicShader::fragmentSource = 
  "#version 330\n"
  "in vec3 color;\n"
  "in vec3 worldPosition;\n"
  "in vec4 viewPosition;\n"
  "out vec4 fragColor;\n"
  "uniform vec3 albedo;\n"
  FOG_SHADER_CHUNK
  "void main() {\n"
  "  fragColor = vec4(fog(color * albedo), 1.0);\n"
  "}\n";

BasicShader::BasicShader() : Shader(vertexSource, fragmentSource) {

}

#include "voxelsShader.hpp"

const char *VoxelsShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPosition;\n"
  "layout (location = 1) in vec3 vColor;\n"
  "layout (location = 2) in vec2 vUV;\n"
  "out vec3 color;\n"
  "out vec2 uv;\n"
  "out vec3 worldPosition;\n"
  "out vec4 viewPosition;\n"
  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main() {\n"
  "  color = vColor;\n"
  "  uv = vUV;\n"
  "  worldPosition = vPosition;\n"
  "  viewPosition = view * model * vec4(vPosition, 1.0);\n"
  "  gl_Position = projection * viewPosition;\n"
  "}\n";

const char *VoxelsShader::fragmentSource = 
  "#version 330\n"
  "in vec3 color;\n"
  "in vec2 uv;\n"
  "in vec3 worldPosition;\n"
  "in vec4 viewPosition;\n"
  "out vec4 fragColor;\n"
  "uniform vec3 albedo;\n"
  "uniform sampler2D diffuse;\n"
  "uniform vec3 fogColor;\n"
  "uniform float fogDensity;\n"
  "void main() {\n"
  "  float fogDepth = length(viewPosition);\n"
  "  float fogFactor = 1.0 - exp(-fogDensity * fogDensity * fogDepth * fogDepth);\n"
  "  fragColor = vec4(mix(vec3(texture2D(diffuse, uv)) * color * albedo, fogColor, fogFactor), 1.0);\n"
  "}\n";

VoxelsShader::VoxelsShader() : Shader(vertexSource, fragmentSource) {

}

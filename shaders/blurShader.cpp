#include "blurShader.hpp"
#include "shaderChunks.hpp"

const char *BlurShader::vertexSource = 
  "#version 330\n"
  "layout (location = 0) in vec3 vPosition;\n"
  "layout (location = 1) in vec2 vUV;\n"
  "out vec2 uv;\n"
  "void main() {\n"
  "  uv = vUV;\n"
  "  gl_Position = vec4(vPosition, 1.0);\n"
  "}\n";

const char *BlurShader::fragmentSource = 
  "#version 330\n"
  "in vec2 uv;\n"
  "out vec4 fragColor;\n"
  "uniform bool flag;\n"
  "uniform vec2 resolution;\n"
  "uniform sampler2DMS colorTexture;\n"
  "const float weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);\n"
  TEXTURE2DMS_SHADER_CHUNK
  "void main() {\n"
  "  ivec2 pixel = ivec2(uv * resolution);\n"
  "  vec3 color = texture2DMS(colorTexture, pixel).rgb * weights[0];\n"
  "  if (flag) {\n"
  "    for(int i = 1; i < 5; i++) {\n"
  "      color += texture2DMS(colorTexture, pixel + ivec2(i, 0)).rgb * weights[i];\n"
  "      color += texture2DMS(colorTexture, pixel - ivec2(i, 0)).rgb * weights[i];\n"
  "    }\n"
  "  } else {\n"
  "    for(int i = 1; i < 5; i++) {\n"
  "      color += texture2DMS(colorTexture, pixel + ivec2(0, i)).rgb * weights[i];\n"
  "      color += texture2DMS(colorTexture, pixel - ivec2(0, i)).rgb * weights[i];\n"
  "    }\n"
  "  }\n"
  "  fragColor = vec4(color, 1.0);\n"
  "}\n";

BlurShader::BlurShader() : Shader(vertexSource, fragmentSource) {

}

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
  "uniform sampler2D colorTexture;\n"
  "const float weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);\n"
  "void main() {\n"
  "  vec2 texel = 1.0 / resolution;"
  "  vec3 color = texture2D(colorTexture, uv).rgb * weights[0];\n"
  "  if (flag) {\n"
  "    for(int i = 1; i < 5; i++) {\n"
  "      color += texture2D(colorTexture, uv + vec2(texel.x * i, 0.0)).rgb * weights[i];\n"
  "      color += texture2D(colorTexture, uv - vec2(texel.x * i, 0.0)).rgb * weights[i];\n"
  "    }\n"
  "  } else {\n"
  "    for(int i = 1; i < 5; i++) {\n"
  "      color += texture2D(colorTexture, uv + vec2(0.0, texel.x * i)).rgb * weights[i];\n"
  "      color += texture2D(colorTexture, uv - vec2(0.0, texel.x * i)).rgb * weights[i];\n"
  "    }\n"
  "  }\n"
  "  fragColor = vec4(color, 1.0);\n"
  "}\n";

BlurShader::BlurShader() : Shader(vertexSource, fragmentSource) {

}

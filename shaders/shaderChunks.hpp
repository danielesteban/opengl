#define FOG_SHADER_CHUNK \
  "uniform vec3 fogColor;\n" \
  "uniform float fogDensity;\n" \
  "vec3 fog(vec3 color) {\n" \
  "  float fogDepth = length(viewPosition);\n" \
  "  float fogFactor = 1.0 - exp(-fogDensity * fogDensity * fogDepth * fogDepth);\n" \
  "  return mix(color, fogColor, fogFactor);\n" \
  "}\n"

#define LINEARIZEDEPTH_SHADER_CHUNK \
  "float nearDepth = 0.1;\n" \
  "float farDepth = 1000.0;\n" \
  "float linearizeDepth(float depth) {\n" \
  "  float z = depth * 2.0 - 1.0;\n" \
  "  return (2.0 * nearDepth * farDepth) / (farDepth + nearDepth - z * (farDepth - nearDepth));\n" \
  "}\n"

#define TEXTURE2DMS_SHADER_CHUNK \
  "uniform int samples;" \
  "vec4 texture2DMS(sampler2DMS sampler, ivec2 coord) {\n" \
  "  vec4 color = vec4(0.0);\n" \
  "  for (int i = 0; i < samples; i++) {\n" \
  "    color += texelFetch(sampler, coord, i);\n" \
  "  }" \
  "  color /= float(samples);\n" \
  "  return color;\n" \
  "}\n"
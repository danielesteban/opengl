#define FOG_SHADER_CHUNK \
  "uniform vec3 fogColor;\n" \
  "uniform float fogDensity;\n" \
  "vec3 fog(vec3 color) {\n" \
  "  float fogDepth = length(viewPosition);\n" \
  "  float fogFactor = 1.0 - exp(-fogDensity * fogDensity * fogDepth * fogDepth);\n" \
  "  return mix(color, fogColor, fogFactor);\n" \
  "}\n"

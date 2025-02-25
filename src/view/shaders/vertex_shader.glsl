#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform vec3 uniformColor;
uniform float flagPaint;

out vec3 fragColor;
out float paintRadius;  // Передаём флаг в фрагментный шейдер

void main() {
  gl_Position = projection * model * vec4(position, 1.0);
  fragColor = uniformColor;
  paintRadius = flagPaint;
}

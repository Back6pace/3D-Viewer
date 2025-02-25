#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform vec3 uniformColor;  // Новый юниформ для цвета рёбер
uniform vec3 vertexColor;   // Новый юниформ для цвета вершин

out vec3 edgeColor;  // Передаем цвет рёбер в геометрический шейдер

void main() {
  gl_Position = projection * model * vec4(position, 1.0);
  edgeColor = uniformColor;  // Цвет рёбер
}

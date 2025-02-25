#version 330 core

in vec3 fragColor;  // Получаем цвет из геометрического шейдера
out vec4 FragColor;

void main() {
  FragColor = vec4(fragColor, 1.0);  // Устанавливаем цвет пикселя
}
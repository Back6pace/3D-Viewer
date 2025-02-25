#version 330 core

in vec3 fragColor;     // Получаем цвет из вершинного шейдера
in float paintRadius;  // Получаем флаг для отрисовки кружками

out vec4 FragColor;

void main() {
  if (paintRadius > 0.0) {
    float dist = length(gl_PointCoord - vec2(0.5));  // Центрируем координаты

    if (dist > 0.5) {  // Обрезаем пиксели за границами круга
      discard;
    }
    FragColor = vec4(fragColor, 1.0);
  } else {
    FragColor = vec4(fragColor, 1.0);
  }
}

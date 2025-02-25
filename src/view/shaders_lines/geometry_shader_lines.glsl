#version 330 core

layout(lines) in;  // Тип входных данных — линии
layout(triangle_strip,
       max_vertices = 4) out;  // Тип выходных данных — прямоугольники

uniform float lineThickness;      // Толщина линии
uniform float lineThicknessCoef;  // Тонкая настрока толщины линии

// Входные данные от вершинного шейдера
in vec3 edgeColor[];  // Цвет для рёбер

out vec3 fragColor;  // Передаем цвет в фрагментный шейдер

void main() {
  vec2 direction = normalize(gl_in[1].gl_Position.xy -
                             gl_in[0].gl_Position.xy);  // Направление линии
  vec2 normal = vec2(-direction.y, direction.x) * lineThickness *
                lineThicknessCoef;  // Уменьшаем толщину линии

  // Передаем цвет рёбер в фрагментный шейдер
  fragColor = edgeColor[0];  // Мы будем использовать цвет первого узла линии
                             // для всего рёбра

  // 1-й треугольник
  gl_Position = vec4(gl_in[0].gl_Position.xy + normal, gl_in[0].gl_Position.zw);
  EmitVertex();

  // 2-й треугольник
  gl_Position = vec4(gl_in[1].gl_Position.xy + normal, gl_in[1].gl_Position.zw);
  EmitVertex();

  // 3-й треугольник
  gl_Position = vec4(gl_in[0].gl_Position.xy - normal, gl_in[0].gl_Position.zw);
  EmitVertex();

  // 4-й треугольник
  gl_Position = vec4(gl_in[1].gl_Position.xy - normal, gl_in[1].gl_Position.zw);
  EmitVertex();

  EndPrimitive();
}

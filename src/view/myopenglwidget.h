#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include "controllerFacade.h"
#include "modelBuilder.h"
#include "settings.h"

namespace s21 {
struct ColorRGB {
  float red;
  float green;
  float blue;
};

struct Settings {
  ColorRGB edges = {1, 1, 1};
  int edges_type = 0;
  float edges_thickness = 1.0f;
  ColorRGB vertices = {1, 1, 1};
  int vertices_type = 0;
  float vertices_size = 1.0f;
  ColorRGB background = {0, 0, 0};
  int projection_type = 0;
  float object_size = 1.0f;
  float translate_x = 0.0f;
  float translate_y = 0.0f;
  float translate_z = 0.0f;
  float rotate_x = 0.0f;
  float rotate_y = 0.0f;
  float rotate_z = 0.0f;
};
}  // namespace s21

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  explicit MyOpenGLWidget(QWidget* parent = nullptr);
  ~MyOpenGLWidget();

  void initializeGL() override;
  void paintGL() override;
  void SetupBuffers();
  void ParseFile(const std::string& file_path);
  void BuildVertices();
  void PaintVertices();
  void PaintEdges();
  void PaintObject();
  void PaintProjection(int type);

  void InitShaders();

  s21::Settings get_settings() { return settings_; }

  void set_vertices(std::vector<float>& vertices) { vertices_ = vertices; }
  void set_edges(const std::vector<unsigned int>& edges) { edges_ = edges; }
  void set_controller(s21::ControllerFacade* controller);

  void set_background_color(s21::ColorRGB background);
  void set_vertices_color(s21::ColorRGB vertices);
  void set_edges_color(s21::ColorRGB edges);
  void set_edges_thickness(int thickness);
  void set_vertices_thickness(int thickness);

  void set_object_size(int size);
  void set_translate_x(int size);
  void set_translate_y(int value);
  void set_translate_z(int value);
  void set_rotate_x(int angle);
  void set_rotate_y(int angle);
  void set_rotate_z(int angle);

  void MoveModel(s21::Mode mode, float value, bool is_click);
  void ScaleModel(float value, bool is_click);
  void RotateModel(s21::Mode mode, float value, bool is_click);

  void set_projection(int type);
  void set_display_method_vertices(int method);
  void set_display_method_edges(int method);
  void AffineTransform(s21::Mode mode, float value, bool is_click);

  void Normalize();

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  QColor bg_color_ = Qt::black;
  std::vector<float> vertices_;
  std::vector<unsigned int> edges_;
  s21::ControllerFacade* controller_;
  s21::Settings settings_;
  float move_x_coef_, move_y_coef_, move_z_coef_ = 0.0f;
  float rotate_x_coef_, rotate_y_coef_, rotate_z_coef_ = 0.0f;
  float prev_scale_ = 10.0f;
  float prev_move_x_ = 10.0f;

  QOpenGLShaderProgram shaderProgramWithoutGS;
  QOpenGLShaderProgram shaderProgramWithGS;
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
  QOpenGLBuffer ebo;

  QMatrix4x4 modelMatrix;
  QMatrix4x4 projectionMatrix;
  GLfloat normalize_coef_ = -10;
  QPoint current_pos_, new_pos_;
  float x_coef_ = -10, y_coef_ = -10, z_coef_ = -10;
};

#endif  // MYOPENGLWIDGET_H

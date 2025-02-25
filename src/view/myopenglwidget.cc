#include "myopenglwidget.h"

#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), controller_(nullptr) {}

MyOpenGLWidget::~MyOpenGLWidget() {
  vertices_.clear();
  edges_.clear();
}

void MyOpenGLWidget::set_controller(s21::ControllerFacade* controller) {
  controller_ = controller;
}

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  InitShaders();
  SetupBuffers();
}

void MyOpenGLWidget::InitShaders() {
  if (!shaderProgramWithoutGS.addShaderFromSourceFile(
          QOpenGLShader::Vertex, "view/shaders/vertex_shader.glsl"))
    std::cerr << "Ошибка компиляции вершинного шейдера:" << std::endl;
  if (!shaderProgramWithoutGS.addShaderFromSourceFile(
          QOpenGLShader::Fragment, "view/shaders/fragment_shader.glsl"))
    std::cerr << "Ошибка компиляции фрагментного шейдера:" << std::endl;
  if (!shaderProgramWithoutGS.link())
    std::cerr << "Ошибка линковки шейдеров:" << std::endl;

  shaderProgramWithGS.addShaderFromSourceFile(
      QOpenGLShader::Vertex, "view/shaders_lines/vertex_shader_lines.glsl");
  shaderProgramWithGS.addShaderFromSourceFile(
      QOpenGLShader::Fragment, "view/shaders_lines/fragment_shader_lines.glsl");
  shaderProgramWithGS.addShaderFromSourceFile(
      QOpenGLShader::Geometry, "view/shaders_lines/geometry_shader_lines.glsl");
  if (!shaderProgramWithGS.link()) {
    std::cerr << "Error linking shader program" << std::endl;
  }
  std::cerr << "Shader program linked" << std::endl;
}
void MyOpenGLWidget::SetupBuffers() {
  vao.create();
  vao.bind();

  vbo.create();
  vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vbo.bind();

  ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  ebo.create();
  ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  ebo.bind();

  shaderProgramWithoutGS.enableAttributeArray(0);
  shaderProgramWithoutGS.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

  shaderProgramWithGS.enableAttributeArray(0);
  shaderProgramWithGS.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

  shaderProgramWithoutGS.release();
  shaderProgramWithGS.release();

  vao.release();
  vbo.release();
  ebo.release();
}
void MyOpenGLWidget::paintGL() {
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "Paint Gl called" << std::endl;
  }

  glClearColor(settings_.background.red, settings_.background.green,
               settings_.background.blue, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  projectionMatrix.setToIdentity();
  modelMatrix.setToIdentity();
  PaintProjection(settings_.projection_type);

  // Используем VAO и рисуем
  vao.bind();
  PaintVertices();
  PaintEdges();
  vao.release();
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event) {
  current_pos_ = event->globalPosition().toPoint();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  new_pos_ = QPoint(event->globalPosition().toPoint() - current_pos_);

  if (event->buttons() & Qt::LeftButton) {
    AffineTransform(s21::kMoveX, static_cast<float>(new_pos_.x()) / 2000, true);
    AffineTransform(s21::kMoveY, static_cast<float>(-new_pos_.y()) / 2000,
                    true);
  } else if (event->buttons() & Qt::RightButton) {
    AffineTransform(s21::kRotateX, -new_pos_.y() * 0.005, true);
    AffineTransform(s21::kRotateY, new_pos_.x() * 0.005, true);
  }
}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event) {
  QPoint numDegrees = event->angleDelta() / 120;
  AffineTransform(s21::kScale, static_cast<float>(numDegrees.y()), true);
}

void MyOpenGLWidget::set_projection(int type) {
  settings_.projection_type = type;
  update();
}

void MyOpenGLWidget::PaintProjection(int type) {
  float near_plane = 0.1f;
  float far_plane = 1000.0f * normalize_coef_;
  if (type == 1) {
    projectionMatrix.ortho(-normalize_coef_, normalize_coef_, -normalize_coef_,
                           normalize_coef_, near_plane, far_plane);
    modelMatrix.translate(0.0f, -normalize_coef_ / 2, -10.0f);
  } else if (type == 0) {
    projectionMatrix.perspective(45.0f, 1.0f, near_plane, far_plane);
    modelMatrix.translate(0.0f, 0.0f, -5.0f * normalize_coef_);
    modelMatrix.rotate(30.0f, 1.0f, 0.0f, 0.0f);
  }
}

void MyOpenGLWidget::ParseFile(const std::string& file_path) {
  controller_->ParseFile(file_path);
  set_vertices(controller_->get_vertices());
  set_edges(controller_->get_edges());
  Normalize();
  BuildVertices();

  if (settings::kDebugMode) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set vertices, size: " << vertices_.size()
               << std::endl;
    for (size_t i = 0; i < vertices_.size(); ++i) {
      debug_file << vertices_[i] << " ";

      if ((i + 1) % 3 == 0) {
        debug_file << std::endl;
      }
    }
    if (vertices_.size() % 3 != 0) {
      debug_file << std::endl;
    }

    debug_file << "\nOpenGL: set edges, size: " << edges_.size() << std::endl;

    for (size_t i = 0; i < edges_.size(); i += 2) {
      debug_file << "Edge " << edges_[i] << " " << edges_[i + 1] << std::endl;
    }

    debug_file << "Parse file Paint Gl called" << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::Normalize() {
  for (size_t i = 0; i < vertices_.size(); i += 3) {
    x_coef_ = std::max(x_coef_, std::abs(vertices_[i]));      // x
    y_coef_ = std::max(y_coef_, std::abs(vertices_[i + 1]));  // y
    z_coef_ = std::max(z_coef_, std::abs(vertices_[i + 2]));  // z
  }
  normalize_coef_ = (x_coef_ + y_coef_ + z_coef_) / 3;
}

void MyOpenGLWidget::BuildVertices() {
  // Debug
  if (settings::kDebugMode) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: build vertices, size: " << vertices_.size()
               << std::endl;
    for (size_t i = 0; i < vertices_.size(); ++i) {
      debug_file << vertices_[i] << " ";

      if ((i + 1) % 3 == 0) {
        debug_file << std::endl;
      }
    }
    if (vertices_.size() % 3 != 0) {
      debug_file << std::endl;
    }

    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
  vao.bind();
  vbo.bind();
  vbo.allocate(vertices_.data(), vertices_.size() * sizeof(float));
  vao.release();
  vbo.release();
}

void MyOpenGLWidget::PaintVertices() {
  shaderProgramWithoutGS.bind();
  shaderProgramWithoutGS.setUniformValue(
      "uniformColor",
      QVector3D(settings_.vertices.red, settings_.vertices.green,
                settings_.vertices.blue));
  shaderProgramWithoutGS.setUniformValue("model", modelMatrix);
  shaderProgramWithoutGS.setUniformValue("projection", projectionMatrix);
  if (settings_.vertices_type == 1) {
    shaderProgramWithoutGS.setUniformValue("flagPaint", 1.0f);
    glPointSize(settings_.vertices_size);
    glDrawArrays(GL_POINTS, 0, vertices_.size() / 3);
  }

  if (settings_.vertices_type == 2) {
    shaderProgramWithoutGS.setUniformValue("flagPaint", 0.0f);
    glPointSize(settings_.vertices_size);
    glDrawArrays(GL_POINTS, 0, vertices_.size() / 3);
  }

  shaderProgramWithoutGS.release();
}

void MyOpenGLWidget::PaintEdges() {
  shaderProgramWithGS.bind();
  shaderProgramWithGS.setUniformValue(
      "uniformColor", QVector3D(settings_.edges.red, settings_.edges.green,
                                settings_.edges.blue));
  shaderProgramWithGS.setUniformValue("model", modelMatrix);
  shaderProgramWithGS.setUniformValue("projection", projectionMatrix);

  if (settings_.edges_type == 0) {
    if (settings_.projection_type == 0) {
      shaderProgramWithGS.setUniformValue("lineThickness",
                                          settings_.edges_thickness);
      shaderProgramWithGS.setUniformValue("lineThicknessCoef", 0.05f);
    } else {
      shaderProgramWithGS.setUniformValue("lineThickness",
                                          settings_.edges_thickness);
      shaderProgramWithGS.setUniformValue("lineThicknessCoef", 0.001f);
    }

    ebo.bind();
    ebo.allocate(edges_.data(), edges_.size() * sizeof(unsigned int));
    glDrawElements(GL_LINES, edges_.size(), GL_UNSIGNED_INT, 0);
    ebo.release();
  } else {
    if (settings_.projection_type == 0) {
      shaderProgramWithGS.setUniformValue("lineThickness",
                                          settings_.edges_thickness);
      shaderProgramWithGS.setUniformValue("lineThicknessCoef", 0.003f);
    } else {
      shaderProgramWithGS.setUniformValue("lineThickness",
                                          settings_.edges_thickness);
      shaderProgramWithGS.setUniformValue("lineThicknessCoef", 0.00004f);
    }

    ebo.bind();
    ebo.allocate(edges_.data(), edges_.size() * sizeof(unsigned int));
    glDrawElements(GL_LINES, edges_.size(), GL_UNSIGNED_INT, 0);
    ebo.release();
  }
  shaderProgramWithGS.release();
}

void MyOpenGLWidget::set_background_color(s21::ColorRGB background) {
  settings_.background.red = background.red;
  settings_.background.green = background.green;
  settings_.background.blue = background.blue;
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set background, red: " << settings_.background.red
               << ", green: " << settings_.background.green
               << ", blue: " << settings_.background.blue << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_vertices_color(s21::ColorRGB vertices) {
  settings_.vertices.red = vertices.red;
  settings_.vertices.green = vertices.green;
  settings_.vertices.blue = vertices.blue;

  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set vertices color, red: "
               << settings_.vertices.red
               << ", green: " << settings_.vertices.green
               << ", blue: " << settings_.vertices.blue << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_edges_color(s21::ColorRGB edges) {
  settings_.edges.red = edges.red;
  settings_.edges.green = edges.green;
  settings_.edges.blue = edges.blue;

  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set edges color, red: " << settings_.edges.red
               << ", green: " << settings_.edges.green
               << ", blue: " << settings_.edges.blue << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_edges_thickness(int thickness) {
  settings_.edges_thickness = static_cast<float>(thickness);

  update();
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set edges thickness, thickness: "
               << settings_.edges_thickness << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_vertices_thickness(int thickness) {
  settings_.vertices_size = static_cast<float>(thickness);

  update();
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set vertices thickness, thickness: "
               << settings_.vertices_size << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_object_size(int size) {
  settings_.object_size = static_cast<float>(size);
  AffineTransform(s21::kScale, settings_.object_size, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set object size, size: " << settings_.object_size
               << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_translate_x(int size) {
  settings_.translate_x = static_cast<float>(size);
  AffineTransform(s21::kMoveX, settings_.translate_x, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set TranslateX:" << settings_.translate_x
               << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_translate_y(int size) {
  settings_.translate_y = static_cast<float>(size);
  AffineTransform(s21::kMoveY, settings_.translate_y, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set TranslateY:" << settings_.translate_y
               << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_translate_z(int size) {
  settings_.translate_z = static_cast<float>(size);
  AffineTransform(s21::kMoveZ, settings_.translate_z, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set TranslateZ:" << settings_.translate_z
               << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_rotate_x(int angle) {
  settings_.rotate_x = static_cast<float>(angle);
  AffineTransform(s21::kRotateX, settings_.rotate_x, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set RotateX:" << settings_.rotate_x << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL) " << std::endl;
  }
}

void MyOpenGLWidget::set_rotate_y(int angle) {
  settings_.rotate_y = static_cast<float>(angle);
  AffineTransform(s21::kRotateY, settings_.rotate_y, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set RotateY:" << settings_.rotate_y << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_rotate_z(int angle) {
  settings_.rotate_z = static_cast<float>(angle);
  AffineTransform(s21::kRotateZ, settings_.rotate_z, false);
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set RotateZ:" << settings_.rotate_z << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_display_method_vertices(int method) {
  settings_.vertices_type = method;

  update();
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set display method vertices, method: "
               << settings_.vertices_type << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::set_display_method_edges(int method) {
  settings_.edges_type = method;

  update();
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: set display method edges, method: "
               << settings_.edges_type << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
}

void MyOpenGLWidget::AffineTransform(s21::Mode mode, float value,
                                     bool is_click) {
  if (mode == s21::kScale) {
    ScaleModel(value, is_click);
  }

  if (mode == s21::kMoveX || mode == s21::kMoveY || mode == s21::kMoveZ) {
    MoveModel(mode, value, is_click);
  }

  if (mode == s21::kRotateX || mode == s21::kRotateY || mode == s21::kRotateZ) {
    RotateModel(mode, value, is_click);
  }
  if (settings::kDebugMode || settings::kDebugView) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nOpenGL: AffineTransform mode: " << mode << std::endl;
    debug_file.close();
    std::cout << "Information saved in debug.log(OpenGL)" << std::endl;
  }
  // Обновление вершинного буфера
  vao.bind();
  vbo.bind();
  vbo.allocate(vertices_.data(), vertices_.size() * sizeof(float));
  vao.release();
  vbo.release();

  update();
}

void MyOpenGLWidget::MoveModel(s21::Mode mode, float value, bool is_click) {
  float norm_coef = 0.0f;
  if (mode == s21::kMoveX) {
    norm_coef = move_x_coef_;
    if (!is_click) move_x_coef_ = value;
  } else if (mode == s21::kMoveY) {
    norm_coef = move_y_coef_;
    if (!is_click) move_y_coef_ = value;
  } else if (mode == s21::kMoveZ) {
    norm_coef = move_z_coef_;
    if (!is_click) move_z_coef_ = value;
  }
  if (!is_click) value = (value - norm_coef) * 0.1;
  controller_->TransformModel(mode, vertices_, value);
}

void MyOpenGLWidget::ScaleModel(float value, bool is_click) {
  double scale_coef = prev_scale_;
  if (is_click && (int)(prev_scale_ + value) > 0) {
    prev_scale_ += value;
    controller_->TransformModel(s21::kScale, vertices_,
                                prev_scale_ / scale_coef);
  } else if (!is_click) {
    float scale_coef = value / prev_scale_;
    prev_scale_ = value;
    controller_->TransformModel(s21::kScale, vertices_, scale_coef);
  }
}

void MyOpenGLWidget::RotateModel(s21::Mode mode, float value, bool is_click) {
  float norm_coef = 0.0f;
  if (mode == s21::kRotateX) {
    norm_coef = rotate_x_coef_;
    if (!is_click) rotate_x_coef_ = value;
  } else if (mode == s21::kRotateY) {
    norm_coef = rotate_y_coef_;
    if (!is_click) rotate_y_coef_ = value;
  } else if (mode == s21::kRotateZ) {
    norm_coef = rotate_z_coef_;
    if (!is_click) rotate_z_coef_ = value;
  }
  if (!is_click) value = (value - norm_coef);
  controller_->TransformModel(mode, vertices_, value);
}

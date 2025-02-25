#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(ControllerFacade* c, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller_(c) {
  ui->setupUi(this);
  setWindowTitle("3D Viewer V2.0");
  ui->openGLWidget->set_controller(controller_);
  // setFixedSize(width(), height());
  ui->timerForGif->setVisible(false);
  settings_ = new QSettings("setting3DViewer.conf", "settings", this);
  LoadSettings();
}

MainWindow::~MainWindow() {
  SaveSettings();
  delete ui;
}

void MainWindow::on_open_file_button_clicked() {
  QString filter_file = "OBJ Files (*.obj)";
  QString file_path = QFileDialog::getOpenFileName(
      this, "Open file", QDir::homePath(), filter_file);

  if (!file_path.isEmpty()) {
    QString file_name = file_path.mid(file_path.lastIndexOf('/') + 1);
    std::string file_path_std = file_path.toStdString();

    ui->openGLWidget->ParseFile(file_path_std);

    // Устанавливаем путь и имя файла в UI и вершины с ребрами
    ui->path_file->setText(file_path);
    ui->name_file->setText(file_name);
    ui->vertices_obj->setText(
        QString::number(controller_->get_vertices().size() / 3));
    ui->edges_obj->setText(QString::number(controller_->get_edges_count()));
  } else {
    QMessageBox::warning(this, "Ошибка", "Файл не был выбран.");
  }
}

void MainWindow::on_background_palitra_clicked() {
  QColor color =
      QColorDialog::getColor(QColor(255, 100, 200, 255), this, "Выберите цвет");
  // Проверяем, был ли выбран цвет
  if (color.isValid()) {
    s21::ColorRGB background_color;
    background_color.red = color.redF();
    background_color.green = color.greenF();
    background_color.blue = color.blueF();
    ui->openGLWidget->set_background_color(background_color);
  }
}

void MainWindow::on_reset_backround_btn_clicked() {
  ui->openGLWidget->set_background_color({0.0f, 0.0f, 0.0f});
  ui->openGLWidget->update();
}

void MainWindow::on_edges_palitra_clicked() {
  QColor color =
      QColorDialog::getColor(QColor(255, 100, 200, 255), this, "Выберите цвет");

  // Проверяем, был ли выбран цвет
  if (color.isValid()) {
    s21::ColorRGB edges_color;
    edges_color.red = color.redF();
    edges_color.green = color.greenF();
    edges_color.blue = color.blueF();
    ui->openGLWidget->set_edges_color(edges_color);
  }
}

void MainWindow::on_reset_edges_btn_clicked() {
  ui->openGLWidget->set_edges_color({1.0f, 1.0f, 1.0f});
  ui->openGLWidget->set_edges_thickness(1.0f);
  ui->thinkness_spin_edge->setValue(1);
  ui->openGLWidget->update();
}

void MainWindow::on_vertices_palitra_clicked() {
  QColor color =
      QColorDialog::getColor(QColor(255, 100, 200, 255), this, "Выберите цвет");

  // Проверяем, был ли выбран цвет
  if (color.isValid()) {
    s21::ColorRGB vertices_color;
    vertices_color.red = color.redF();
    vertices_color.green = color.greenF();
    vertices_color.blue = color.blueF();
    ui->openGLWidget->set_vertices_color(vertices_color);
  }
}

void MainWindow::on_reset_vertices_btn_clicked() {
  ui->openGLWidget->set_vertices_color({1.0f, 1.0f, 1.0f});
  ui->openGLWidget->set_vertices_thickness(1.0f);
  ui->thinkness_spin_vertices->setValue(1);
  ui->openGLWidget->update();
}

void MainWindow::on_thinkness_spin_edge_valueChanged(int arg1) {
  ui->openGLWidget->set_edges_thickness(arg1);
}

void MainWindow::on_thinkness_spin_vertices_valueChanged(int arg1) {
  ui->openGLWidget->set_vertices_thickness(arg1);
}

void MainWindow::on_scale_slider_valueChanged(int value) {
  ui->openGLWidget->set_object_size(value);
}

void MainWindow::on_move_x_slider_valueChanged(int value) {
  ui->openGLWidget->set_translate_x(value);
}

void MainWindow::on_move_y_slider_valueChanged(int value) {
  ui->openGLWidget->set_translate_y(value);
}

void MainWindow::on_move_z_slider_valueChanged(int value) {
  ui->openGLWidget->set_translate_z(value);
}

void MainWindow::on_rotate_x_valueChanged(int value) {
  ui->openGLWidget->set_rotate_x(value);
}

void MainWindow::on_rotate_y_valueChanged(int value) {
  ui->openGLWidget->set_rotate_y(value);
}

void MainWindow::on_rotate_z_valueChanged(int value) {
  ui->openGLWidget->set_rotate_z(value);
}

void MainWindow::on_display_method_vertices_box_currentIndexChanged(int index) {
  ui->openGLWidget->set_display_method_vertices(index);
}

void MainWindow::on_display_method_edges_box_currentIndexChanged(int index) {
  ui->openGLWidget->set_display_method_edges(index);
}
void MainWindow::on_projection_box_currentIndexChanged(int index) {
  ui->openGLWidget->set_projection(index);
}

void MainWindow::on_screenshot_btn_clicked() {
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  update();
  QString file_path = QFileDialog::getSaveFileName(
      this, "Save Screenshot", "screenshot", "JPEG (*.jpeg);;BMP (*.bmp)");
  if (!file_path.isEmpty()) {
    if (file_path.endsWith(".bmp"))
      screenshot.save(file_path, "BMP");
    else
      screenshot.save(file_path, "JPEG");
  }
}

void MainWindow::on_gif_btn_clicked() {
  ui->timerForGif->setVisible(true);
  ui->timerForGif->setText("Gif start...");
  tempFolder_ = QCoreApplication::applicationDirPath() + "/temp_frames/";
  QDir().mkpath(tempFolder_);
  frameCount_ = 0;
  gifTimer_ = new QTimer(this);
  connect(gifTimer_, &QTimer::timeout, this, &MainWindow::CaptureFrame);
  gifTimer_->start(100);  // 10 кадров в секунду (100 мс между кадрами)

  QTimer::singleShot(5000, this,
                     &MainWindow::StopRecording);  // Останавливаем через 5 сек
}

void MainWindow::CaptureFrame() {
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  QString filename =
      tempFolder_ + QString("frame_%1.png").arg(frameCount_, 3, 10, QChar('0'));
  screenshot.save(filename, "PNG");

  if (settings::kDebugView || settings::kDebugMode) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "Saved frame: " << filename.toStdString();
    debug_file.close();
    std::cout << "Information saved in debug.log(MainWindow)" << std::endl;
  }
  frameCount_++;
}

void MainWindow::StopRecording() {
  gifTimer_->stop();
  delete gifTimer_;
  ui->timerForGif->setText("Gif END");
  ui->timerForGif->setVisible(true);

  QTimer::singleShot(3000, this, [this]() {
    ui->timerForGif->setVisible(false);  // Скрываем после 3 секунд
  });

  QString ffmpegPath =
      QCoreApplication::applicationDirPath() + "/../../src/ffmpeg/lib/ffmpeg";

  QString outputGif = QFileDialog::getSaveFileName(
      this, tr("Save GIF"),
      QCoreApplication::applicationDirPath() + "/output.gif",  // начальный путь
      tr("GIF Files (*.gif);;All Files (*)"));

  if (outputGif.isEmpty()) {
    qDebug() << "No file selected!";
    return;
  }

  QStringList arguments;
  arguments << "-y" << "-framerate" << "10"
            << "-i" << tempFolder_ + "frame_%03d.png" << "-vf"
            << "scale=640:480" << outputGif;

  QProcess* process = new QProcess();
  process->start(ffmpegPath, arguments);

  if (!process->waitForFinished()) {
    qDebug() << "FFmpeg error:" << process->errorString();
  } else {
    qDebug() << "GIF saved as:" << outputGif;
  }

  QDir dir(tempFolder_);
  dir.removeRecursively();
}

void MainWindow::SaveSettings() {
  Settings settings = ui->openGLWidget->get_settings();
  settings_->setValue("settings", 1);  // Flag for checking in LoadSettings()
  // Edges
  settings_->setValue("edges_type", settings.edges_type);
  settings_->setValue("edges_thickness", settings.edges_thickness);
  settings_->setValue("edges_red", settings.edges.red);
  settings_->setValue("edges_green", settings.edges.green);
  settings_->setValue("edges_blue", settings.edges.blue);
  // Vertices
  settings_->setValue("vertices_type", settings.vertices_type);
  settings_->setValue("vertices_size", settings.vertices_size);
  settings_->setValue("vertices_red", settings.vertices.red);
  settings_->setValue("vertices_green", settings.vertices.green);
  settings_->setValue("vertices_blue", settings.vertices.blue);
  // Background
  settings_->setValue("background_red", settings.background.red);
  settings_->setValue("background_green", settings.background.green);
  settings_->setValue("background_blue", settings.background.blue);
  // Projection
  settings_->setValue("projection_type", settings.projection_type);
  // Scale
  settings_->setValue("object_size", settings.object_size);
  // Rotation
  // Translate
}

void MainWindow::LoadSettings() {
  if (settings_->value("settings").toInt() == 1) {
    Settings settings;
    // Edges
    settings.edges_type = settings_->value("edges_type").toInt();
    settings.edges_thickness = settings_->value("edges_thickness").toFloat();
    settings.edges.red = settings_->value("edges_red").toFloat();
    settings.edges.green = settings_->value("edges_green").toFloat();
    settings.edges.blue = settings_->value("edges_blue").toFloat();

    ui->display_method_edges_box->setCurrentIndex(settings.edges_type);
    ui->thinkness_spin_edge->setValue(settings.edges_thickness);

    ui->openGLWidget->set_display_method_edges(settings.edges_type);
    ui->openGLWidget->set_edges_thickness(settings.edges_thickness);
    ui->openGLWidget->set_edges_color(
        {settings.edges.red, settings.edges.green, settings.edges.blue});
    // Vertices
    settings.vertices_type = settings_->value("vertices_type").toInt();
    settings.vertices_size = settings_->value("vertices_size").toFloat();
    settings.vertices.red = settings_->value("vertices_red").toFloat();
    settings.vertices.green = settings_->value("vertices_green").toFloat();
    settings.vertices.blue = settings_->value("vertices_blue").toFloat();

    ui->display_method_vertices_box->setCurrentIndex(settings.vertices_type);
    ui->thinkness_spin_vertices->setValue(settings.vertices_size);

    ui->openGLWidget->set_display_method_vertices(settings.vertices_type);
    ui->openGLWidget->set_vertices_thickness(settings.vertices_size);
    ui->openGLWidget->set_vertices_color({settings.vertices.red,
                                          settings.vertices.green,
                                          settings.vertices.blue});
    // Background
    settings.background.red = settings_->value("background_red").toFloat();
    settings.background.green = settings_->value("background_green").toFloat();
    settings.background.blue = settings_->value("background_blue").toFloat();

    ui->openGLWidget->set_background_color({settings.background.red,
                                            settings.background.green,
                                            settings.background.blue});
    // Projection
    settings.projection_type = settings_->value("projection_type").toInt();
    ui->projection_box->setCurrentIndex(settings.projection_type);

    ui->openGLWidget->set_projection(settings.projection_type);
  }
}
}  // namespace s21

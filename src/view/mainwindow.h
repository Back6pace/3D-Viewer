#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <QWidget>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

#include "controllerFacade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(ControllerFacade *c, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_open_file_button_clicked();

  void on_screenshot_btn_clicked();
  void on_gif_btn_clicked();
  void on_rotate_x_valueChanged(int value);
  void on_rotate_y_valueChanged(int value);
  void on_rotate_z_valueChanged(int value);
  void on_scale_slider_valueChanged(int value);

  void on_move_x_slider_valueChanged(int value);
  void on_move_y_slider_valueChanged(int value);
  void on_move_z_slider_valueChanged(int value);
  void on_display_method_vertices_box_currentIndexChanged(int index);
  void on_display_method_edges_box_currentIndexChanged(int index);
  void on_thinkness_spin_vertices_valueChanged(int arg1);
  void on_thinkness_spin_edge_valueChanged(int arg1);
  void on_projection_box_currentIndexChanged(int index);
  void on_reset_backround_btn_clicked();
  void on_reset_vertices_btn_clicked();
  void on_reset_edges_btn_clicked();
  void on_background_palitra_clicked();
  void on_vertices_palitra_clicked();
  void on_edges_palitra_clicked();
  void SaveSettings();
  void LoadSettings();

  void CaptureFrame();
  void StopRecording();

 private:
  Ui::MainWindow *ui;
  ControllerFacade *controller_;
  QTimer *gifTimer_;
  QString tempFolder_;
  int frameCount_;
  QSettings *settings_;
};

}  // namespace s21

#endif  // MAINWINDOW_H

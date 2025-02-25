#include <QApplication>

#include "controllerFacade.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setMajorVersion(3);
  format.setMinorVersion(3);
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);

  // Создаем объект Qt-приложения
  QApplication app(argc, argv);
  // Создаем конкретного строителя
  s21::ModelBuilder model_builder;
  // Создаем контроллер, передаем ему билдера
  s21::ControllerFacade controller(model_builder);

  s21::MainWindow view(&controller);
  view.show();
  return app.exec();
}

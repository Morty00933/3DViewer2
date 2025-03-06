#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "model/controller.h"
#include "mainwindow.h"
#include "model/model.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Model model;
  s21::Controller &controller = s21::Controller::getInstance(&model);
  MainWindow w(&controller);
  w.show();
  return a.exec();
}

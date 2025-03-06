#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMainWindow>

#include "openglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *controller, QWidget *parent = nullptr);
  ~MainWindow();
  int open_file = 0;  // База, при которой очистка не требуется

 signals:
  void fileSelected(const QString &file_path);

 private slots:
  void on_selectionFile_clicked();
  void onColorButtonFoneClicked();
  void onColorButtonLineClicked();
  void onColorButtonVerClicked();
  void onLineRadioButtonClicked(bool is_checked_line);
  void onVerRadioButtonClicked(bool is_checked_ver);
  void onSaveBMPButtonClicked();
  void onSaveJPEGButtonClicked();
  void TransferVerticesFacets(int count_vertex, int count_facets);
  void TransferFileIncorrect(QString error_message);
  void ScaleModelFromSpinBox(double scale_factor);
  void IntervalLines(double interval_value);
  void ThicknessLines(double thickness_value);
  void SizeVer(double size_ver);
  void TranslateProjectionType(bool is_check_projection);
  void ClearAllFunc();

 private:
  Ui::MainWindow *ui;
  s21::Controller *controller_;
  OpenGLWidget *glWidget;
  QString file_path;

 public slots:
  void onPushButtonRotateClicked();
  void onPushButtonMoveClicked();
};
#endif  // MAINWINDOW_H

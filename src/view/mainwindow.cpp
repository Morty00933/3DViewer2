#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller_(controller) {
  ui->setupUi(this);

  glWidget = new OpenGLWidget(controller_, ui->display);
  glWidget->setFixedSize(791, 601);  // !!!размерность окна дисплея

  connect(this, SIGNAL(fileSelected(QString)), glWidget,
          SLOT(LoadModelFile(QString)));

  // Для окрашивания
  connect(ui->colorVer, SIGNAL(clicked()), this,
          SLOT(onColorButtonVerClicked()));
  connect(ui->colorFon, SIGNAL(clicked()), this,
          SLOT(onColorButtonFoneClicked()));
  connect(ui->colorLine, SIGNAL(clicked()), this,
          SLOT(onColorButtonLineClicked()));

  // Для получения значений SpinBox
  connect(ui->doubleSpinBox_scal,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::ScaleModelFromSpinBox);
  connect(ui->doubleSpinBox_interval,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::IntervalLines);
  connect(ui->doubleSpinBox_thickness,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::ThicknessLines);
  connect(ui->doubleSpinBox_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::SizeVer);

  // Для допки с ребрами
  connect(ui->dotted_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));
  connect(ui->solid_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));

  // Для допки с вершинами
  connect(ui->Circle, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->Square, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->No_ver, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));

  // Для отображения вершин, граней и некоректного файла
  connect(glWidget, &OpenGLWidget::CountVertexFacets, this,
          &MainWindow::TransferVerticesFacets);
  connect(glWidget, &OpenGLWidget::FileIncorrect, this,
          &MainWindow::TransferFileIncorrect);

  // Для допки сохранения в форматах
  connect(ui->pushButton_bmp, SIGNAL(clicked()), this,
          SLOT(onSaveBMPButtonClicked()));
  connect(ui->pushButton_jpeg, SIGNAL(clicked()), this,
          SLOT(onSaveJPEGButtonClicked()));

  // Для переворота
  connect(ui->pushButton_xProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_xMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);

  // Для перемещения
  connect(ui->pushButton_xPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_xMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);

  // Для проекции
  connect(ui->typeCenter, SIGNAL(toggled(bool)), this,
          SLOT(TranslateProjectionType(bool)));
  connect(ui->typeParalel, SIGNAL(toggled(bool)), this,
          SLOT(TranslateProjectionType(bool)));

  // Очищение
  connect(ui->Clear, SIGNAL(clicked()), this, SLOT(ClearAllFunc()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_selectionFile_clicked() {
  file_path = QFileDialog::getOpenFileName(this, "Выбрать файл", "",
                                           "Wavefront OBJ файлы (*.obj)");
  if (!file_path.isEmpty()) {
    if (open_file == 1 && (ui->label_file->text() != "File incorrect")) {
      glWidget->ClearContent();
    }
    open_file = 1;  // Для отчистки предыдущего объекта
    QFileInfo fileInfo(file_path);
    if (fileInfo.exists()) {
      QString fileName = fileInfo.fileName();
      ui->label_file->setText(fileName);
      emit fileSelected(file_path);
    } else {
      ui->label_file->setText("File doesn't exist");
      open_file = 0;
    }
  } else {
    ui->label_file->setText("File selection canceled");
    open_file = 0;
  }
}

void MainWindow::onPushButtonRotateClicked() {
  if (file_path.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xProtat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yProtat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zProtat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMrotat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMrotat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMrotat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->EnterModelRotation(labelValue, 'Z');
    }
  }
}

void MainWindow::onPushButtonMoveClicked() {
  if (file_path.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xPmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yPmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zPmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->ShiftModel(labelValue, 'Z');
    }
  }
}

void MainWindow::onColorButtonFoneClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->SetBackgroundColor(color);
      ui->colorFon_2->setStyleSheet("background-color: " + color.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonLineClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->SetColorLineVer(color, true);
      ui->colorLine_2->setStyleSheet("background-color: " + color.name() +
                                     ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonVerClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->SetColorLineVer(color, false);
      ui->colorVer_2->setStyleSheet("background-color: " + color.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::TransferVerticesFacets(int count_vertex, int count_facets) {
  ui->label_top->setText(QString::number(count_vertex));
  ui->label_ribs->setText(QString::number(count_facets));
}

void MainWindow::TransferFileIncorrect(QString error_message) {
  ui->label_file->setText(error_message);
}

void MainWindow::ScaleModelFromSpinBox(double scale_factor) {
  glWidget->ScaleModelToFit(scale_factor);
  update();
}

void MainWindow::IntervalLines(double interval_value) {
  glWidget->EditIntervalLines(interval_value);
  update();
}

void MainWindow::ThicknessLines(double thickness_value) {
  glWidget->EditThicknessLines(thickness_value);
  update();
}

void MainWindow::SizeVer(double size_ver) {
  glWidget->EditSizeVer(size_ver);
  update();
}

void MainWindow::onLineRadioButtonClicked(bool is_checked_line) {
  if (is_checked_line) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->dotted_line) {
      glWidget->SetLineStyle(true);
    } else if (selectedRadioButton == ui->solid_line) {
      glWidget->SetLineStyle(false);
    }
    update();
  }
}

void MainWindow::onVerRadioButtonClicked(bool is_checked_ver) {
  if (is_checked_ver) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->Circle) {
      glWidget->VerStyle(1);
    } else if (selectedRadioButton == ui->Square) {
      glWidget->VerStyle(2);
    } else if (selectedRadioButton == ui->No_ver) {
      glWidget->VerStyle(0);
    }
    update();
  }
}

void MainWindow::onSaveBMPButtonClicked() {
  QString file_path =
      QFileDialog::getSaveFileName(this, "Save Image", "", "Bitmap (*.bmp)");
  if (!file_path.isEmpty()) {
    glWidget->SaveImage(file_path);
  }
}

void MainWindow::onSaveJPEGButtonClicked() {
  QString file_path =
      QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG (*.jpeg)");
  if (!file_path.isEmpty()) {
    glWidget->SaveImage(file_path);
  }
}

void MainWindow::TranslateProjectionType(bool is_check_projection) {
  if (is_check_projection) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->typeCenter) {
      glWidget->SetProjectionType(0);
    } else if (selectedRadioButton == ui->typeParalel) {
      glWidget->SetProjectionType(1);
    }
    update();
  }
}

void MainWindow::ClearAllFunc() {
  ui->doubleSpinBox_scal->setValue(1.00);
  if (open_file != 2) {
    if (!file_path.isEmpty() && open_file == 1 &&
        (ui->label_file->text() != "File incorrect")) {
      glWidget->ClearContent();
      open_file = 0;
    }
    ui->label_file->setText("");

    ui->lineEdit_Xrotat->setText("0.0175");
    ui->lineEdit_Yrotat->setText("0.0175");
    ui->lineEdit_Zrotat->setText("0.0175");

    ui->lineEdit_xMove->setText("0.25");
    ui->lineEdit_yMove->setText("0.25");
    ui->lineEdit_zMove->setText("0.25");

    ui->doubleSpinBox_interval->setValue(1.00);
    ui->doubleSpinBox_size->setValue(1.00);
    ui->doubleSpinBox_thickness->setValue(1.00);

    ui->colorFon_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px");
    ui->colorLine_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");
    ui->colorVer_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");

    glWidget->SetBackgroundColor(Qt::black);
    glWidget->SetColorLineVer(Qt::red, false);
    glWidget->SetColorLineVer(Qt::gray, true);

    open_file = 2;

    ui->label_ribs->setText("");
    ui->label_top->setText("");

    glWidget->update();
  }
}

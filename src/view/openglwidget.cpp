#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(s21::Controller *controller, QWidget *parent)
    : QOpenGLWidget(parent),
      controller(controller),
      file_loaded(false),
      scale(1.0),
      point_color(Qt::red),
      line_color(Qt::gray) {
  setFixedSize(win_width, win_height);
}

void OpenGLWidget::initializeGL() { initializeOpenGLFunctions(); }

void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWidget::paintGL() {
  if (!file_loaded) {
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (use_dotted_line) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  if (use_dotted_ver == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }
  if (use_dotted_ver == 2) {
    glEnable(GL_TEXTURE_2D);
  } else {
    glDisable(GL_TEXTURE_2D);
  }
  if (use_dotted_ver != 0) {
    glColor3f(point_color.redF(), point_color.greenF(), point_color.blueF());
    glBegin(GL_POINTS);
    const auto &matrix_3d = controller->GetVertixes();
    for (size_t i = 1; i < matrix_3d.size(); i++) {
      glVertex3dv(matrix_3d[i].data());
    }
    glEnd();
  }
  glColor3f(line_color.redF(), line_color.greenF(), line_color.blueF());
  glBegin(GL_LINES);
  for (const auto &facet : controller->GetFacets()) {
    for (size_t i = 0; i < facet.vertexes.size(); ++i) {
      int currentVertexIndex = facet.vertexes[i];
      int nextVertexIndex = facet.vertexes[(i + 1) % facet.vertexes.size()];
      glVertex3dv(controller->GetVertixes()[currentVertexIndex].data());
      glVertex3dv(controller->GetVertixes()[nextVertexIndex].data());
    }
  }
  glEnd();
  glDisable(GL_LINE_STIPPLE);
  emit CountVertexFacets(controller->GetVertexNumber(),
                         controller->GetFacetNumber());
}

void OpenGLWidget::LoadModelFile(const QString &file_path) {
  try {
    if (file_loaded) {
      controller->ClearModel();
      update();
    }
    controller->LoadModel(file_path.toStdString());
    controller->CenteriseModel();
    ScaleModelToFit(1.0);
    file_loaded = true;
    update();
  } catch (const std::exception &e) {
    controller->ClearModel();  ///////////////////////////////////////////////
    emit FileIncorrect("File incorrect");
    emit CountVertexFacets(0, 0);
  }
}

void OpenGLWidget::SetProjectionType(int value) {
  makeCurrent();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (value == 0) {
    // Центральная
    float fov = 60 * M_PI / 180;
    float heapHeight = win_height / (2 * tan(fov / 2));
    glFrustum(-win_width / 12, win_width / 12, -win_height / 12,
              win_height / 12, heapHeight, 2);
  } else if (value == 1) {
    // Параллельная
    glOrtho(-win_width / win_width, win_width / win_width,
            -win_height / win_height, win_height / win_height, -100, 100);
  }
  glTranslatef(0.0, 0.0, -10.0);
  glMatrixMode(GL_MODELVIEW);
  doneCurrent();
  update();
}

void OpenGLWidget::ScaleModelToFit(double scale_factor) {
  makeCurrent();
  controller->ScaleModelToFit(scale_factor);
  doneCurrent();
  update();
}

void OpenGLWidget::EditIntervalLines(double interval_value) {
  makeCurrent();
  glLineStipple(interval_value, 0x0F0F);
  doneCurrent();
  update();
}

void OpenGLWidget::EditThicknessLines(double thickness_value) {
  makeCurrent();
  glLineWidth(thickness_value);
  doneCurrent();
  update();
}

void OpenGLWidget::SetLineStyle(bool line) {
  makeCurrent();
  use_dotted_line = line;
  doneCurrent();
  update();
}

void OpenGLWidget::VerStyle(int dottedVer) {
  makeCurrent();
  use_dotted_ver = dottedVer;
  doneCurrent();
  update();
}

void OpenGLWidget::EditSizeVer(double size_ver) {
  makeCurrent();
  glPointSize(size_ver);
  doneCurrent();
  update();
}

void OpenGLWidget::SetBackgroundColor(const QColor &color) {
  makeCurrent();
  glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  doneCurrent();
  update();
}

void OpenGLWidget::SetColorLineVer(const QColor &color, bool type) {
  if (type) {
    line_color = color;
  } else {
    point_color = color;
  }
  update();
}

void OpenGLWidget::EnterModelRotation(double step, char xyz) {
  makeCurrent();
  controller->EnterModelRotation(step, xyz);
  controller->ApplyModelRotation();
  doneCurrent();
  update();
}

void OpenGLWidget::ShiftModel(double step, char xyz) {
  makeCurrent();
  controller->ShiftModel(step, xyz);
  doneCurrent();
  update();
}

void OpenGLWidget::SaveImage(const QString &filename) {
  QImage image = grabFramebuffer();
  image.save(filename);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  last_mouse_pos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - last_mouse_pos.x();
  int dy = event->y() - last_mouse_pos.y();
  EnterModelRotation(dy * rotation_speed, 'x');
  EnterModelRotation(dx * rotation_speed, 'y');
  last_mouse_pos = event->pos();
  update();
}

void OpenGLWidget::ClearContent() {
  file_loaded = false;
  controller->ClearModel();
  update();
}
#ifndef CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H
#define CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H

#include "model.h"

namespace s21 {

class Command {
 public:
  virtual void execute() = 0;
};

class Rotate : public Command {
 public:
  Rotate(Model *model, double angle, char axis)
      : axis_(axis), angle_(angle), model_(model) {}
  void execute() override { model_->Rotate(angle_, axis_); }

 private:
  char axis_;
  double angle_;
  Model *model_;
};

class Move : public Command {
 public:
  Move(Model *model, double distance, char axis)
      : axis_(axis), distance_(distance), model_(model) {}
  void execute() override { model_->Move(distance_, axis_); }

 private:
  char axis_;
  double distance_;
  Model *model_;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H

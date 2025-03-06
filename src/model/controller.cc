#include "controller.h"

namespace s21 {

void Controller::LoadModel(const std::string &file_path) {
  model_->LoadModel(file_path);
}

void Controller::EnterModelRotation(double step, char xyz) {
  Rotate cmd(model_, step, xyz);
  cmd.execute();
}

void Controller::ApplyModelRotation() { model_->ApplyRotation(); }

void Controller::ShiftModel(double distance, char xyz) {
  Move cmd(model_, distance, xyz);
  cmd.execute();
}

void Controller::CenteriseModel() { model_->Centerise(); }

void Controller::ScaleModelToFit(double scale_factor) {
  model_->ScaleToFit(scale_factor);
}

void Controller::ClearModel() { model_->ClearModel(); }

}  // namespace s21

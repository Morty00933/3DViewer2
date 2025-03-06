#ifndef CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H
#define CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H

#include <stdexcept>
#include <string>

#include "../model/command.h"
#include "../model/model.h"

namespace s21 {

class Controller {
 public:
  static Controller &getInstance(Model *model = nullptr) {
    static Controller instance(model);
    return instance;
  }

  void LoadModel(const std::string &file_path);
  void EnterModelRotation(double step, char xyz);
  void ApplyModelRotation();
  void ShiftModel(double distance, char xyz);
  void CenteriseModel();
  void ScaleModelToFit(double scale_factor);
  void ClearModel();
  int GetVertexNumber() const { return model_->GetVertexesNumber(); }
  int GetFacetNumber() const { return model_->GetFacetsNumber(); }
  const std::vector<std::vector<double>> &GetVertixes() const {
    return model_->GetVertixes();
  }
  const std::vector<Facet> &GetFacets() const { return model_->GetFacets(); }

 private:
  Controller(Model *model) : model_(model) {}
  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;

  Model *model_;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H
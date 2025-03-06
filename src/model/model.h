#ifndef CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H
#define CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

struct Facet {
  std::vector<int> vertexes;
  int count_vertexes_in_facets;
  bool operator==(const Facet &other) const {
    return (vertexes == other.vertexes &&
            count_vertexes_in_facets == other.count_vertexes_in_facets);
  }
};

class Model {
 public:
  Model();
  ~Model();

  void LoadModel(const std::string &file_path);
  void Rotate(double step, char xyz);
  void ApplyRotation();
  void Move(double distance, char xyz);
  void Centerise();
  void ScaleToFit(double scale_factor);
  void ClearModel();

  int GetVertexesNumber() const { return vertexes_number_; }
  int GetFacetsNumber() const { return facets_number_; }
  const std::vector<std::vector<double>> &GetVertixes() const {
    return vertexes_;
  }
  const std::vector<Facet> &GetFacets() const { return facets_; }

 private:
  void RotatePoint(std::vector<double> &point, double angle, char xyz);
  void CountVertexesFacets(const std::string &file_path);

  int vertexes_number_ = 0;
  int facets_number_ = 0;
  std::vector<std::vector<double>> vertexes_;
  std::vector<Facet> facets_;
  double x_angle_;
  double y_angle_;
  double z_angle_;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H

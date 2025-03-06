#include "model.h"

namespace s21 {

Model::Model()
    : vertexes_number_(0),
      facets_number_(0),
      x_angle_(0.0),
      y_angle_(0.0),
      z_angle_(0.0) {}

Model::~Model() { ClearModel(); }

void Model::CountVertexesFacets(const std::string &file) {
  std::ifstream ifs(file);
  if (!ifs.is_open())
    throw std::runtime_error("File error: failed to open file");
  facets_number_ = 0;
  vertexes_number_ = 0;
  std::string line;
  while (std::getline(ifs, line)) {
    if (line.substr(0, 2) == "v ") {
      vertexes_number_++;
    } else if (line.substr(0, 2) == "f ") {
      facets_number_++;
    }
  }
  ifs.close();
}

void Model::LoadModel(const std::string &file) {
  CountVertexesFacets(file);
  std::ifstream ifs(file);
  if (!ifs.is_open())
    throw std::runtime_error("File error: failed to open file");
  std::string line;
  int vertex_i = 0, facet_i = 0, count_vertex = 0;
  vertexes_.resize(vertexes_number_ + 1, std::vector<double>(3, 0.0));
  facets_.resize(facets_number_);
  while (std::getline(ifs, line)) {
    if (line.substr(0, 2) == "v ") {
      vertex_i++;
      std::istringstream iss(line.substr(2));
      std::vector<double> coords;
      double coord = 0;
      while (iss >> coord) coords.push_back(coord);
      if (coords.size() != 3)
        throw std::runtime_error(
            "File error: vertex must have exactly 3 coordinates");
      vertexes_[vertex_i][0] = coords[0];
      vertexes_[vertex_i][1] = coords[1];
      vertexes_[vertex_i][2] = coords[2];
      count_vertex++;
    } else if (line.substr(0, 2) == "f ") {
      std::istringstream iss(line.substr(2));
      std::string token;
      int count_vertexes_in_facets = 0;
      while (iss >> token) {
        int curr_vertex_i = std::stoi(token);
        if (curr_vertex_i < 0) curr_vertex_i = count_vertex + 1 + curr_vertex_i;
        if (curr_vertex_i == 0 || curr_vertex_i > vertex_i)
          throw std::runtime_error("File error: vertex index out of range");
        else {
          facets_[facet_i].vertexes.push_back(curr_vertex_i);
          count_vertexes_in_facets++;
        }
      }
      facets_[facet_i].count_vertexes_in_facets = count_vertexes_in_facets;
      if (count_vertexes_in_facets > 1) facet_i++;
    }
  }
  ifs.close();
}

void Model::Rotate(double angle, char axis) {
  switch (axis) {
    case 'x':
      x_angle_ = angle;
      break;
    case 'y':
      y_angle_ = angle;
      break;
    case 'z':
      z_angle_ = angle;
      break;
    case 'X':
      x_angle_ = -angle;
      break;
    case 'Y':
      y_angle_ = -angle;
      break;
    case 'Z':
      z_angle_ = -angle;
      break;
  }
}

void Model::RotatePoint(std::vector<double> &point, double angle, char axis) {
  double angle_sin = sin(angle);
  double angle_cos = cos(angle);
  double buff = 0;
  switch (axis) {
    case 'x':
      buff = point[2] * angle_cos + point[1] * angle_sin;
      point[1] = point[1] * angle_cos - point[2] * angle_sin;
      point[2] = buff;
      break;
    case 'y':
      buff = point[2] * angle_cos - point[0] * angle_sin;
      point[0] = point[0] * angle_cos + point[2] * angle_sin;
      point[2] = buff;
      break;
    case 'z':
      buff = point[1] * angle_cos + point[0] * angle_sin;
      point[0] = point[0] * angle_cos - point[1] * angle_sin;
      point[1] = buff;
      break;
  }
}

void Model::ApplyRotation() {
  for (int i = 1; i <= vertexes_number_; i++) {
    RotatePoint(vertexes_[i], x_angle_, 'x');
    RotatePoint(vertexes_[i], y_angle_, 'y');
    RotatePoint(vertexes_[i], z_angle_, 'z');
  }
  x_angle_ = 0.0;
  y_angle_ = 0.0;
  z_angle_ = 0.0;
}

void Model::Move(double distance, char axis) {
  switch (axis) {
    case 'x':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][0] += distance;
      }
      break;
    case 'y':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][1] += distance;
      }
      break;
    case 'z':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][2] += distance;
      }
      break;
    case 'X':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][0] -= distance;
      }
      break;
    case 'Y':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][1] -= distance;
      }
      break;
    case 'Z':
      for (int i = 1; i <= vertexes_number_; i++) {
        vertexes_[i][2] -= distance;
      }
      break;
  }
}

void Model::Centerise() {
  const auto &vertices = this->GetVertixes();
  if (vertices.empty()) throw std::runtime_error("Model error: empty model");
  double center_x = 0.0;
  double center_y = 0.0;
  double center_z = 0.0;
  for (const auto &vertex : vertices) {
    center_x += vertex[0];
    center_y += vertex[1];
    center_z += vertex[2];
  }
  center_x /= vertexes_number_;
  center_y /= vertexes_number_;
  center_z /= vertexes_number_;
  this->Move(-center_x, 'x');
  this->Move(-center_y, 'y');
  this->Move(-center_z, 'z');
}

void Model::ScaleToFit(double scale_factor) {
  if (vertexes_.empty()) throw std::runtime_error("Model error: empty model");
  double max_distance = 0.0;
  for (const auto &vertex : vertexes_) {
    double distance =
        sqrt(pow(vertex[0], 2) + pow(vertex[1], 2) + pow(vertex[2], 2));
    if (distance > max_distance) {
      max_distance = distance;
    }
  }
  if (max_distance > 0.0) {
    double scale = scale_factor / max_distance;
    for (auto &vertex : vertexes_) {
      vertex[0] *= scale;
      vertex[1] *= scale;
      vertex[2] *= scale;
    }
  }
}

void Model::ClearModel() {
  x_angle_ = 0.0;
  y_angle_ = 0.0;
  z_angle_ = 0.0;
  vertexes_.clear();
  facets_.clear();
  vertexes_number_ = 0;
  facets_number_ = 0;
}

}  // namespace s21

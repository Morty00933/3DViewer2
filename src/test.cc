#include <gtest/gtest.h>

#include <iostream>

#include "model/model.h"

namespace s21 {

class ModelTest : public ::testing::Test {
 protected:
  Model *model;

  void SetUp() override { model = new Model(); }

  void TearDown() override { delete model; }
};

TEST_F(ModelTest, CountVertexesFacets) {  // количество вершин и граней

  model->LoadModel("obj/bull.obj");
  EXPECT_EQ(model->GetFacetsNumber(), 49536);
  EXPECT_EQ(model->GetVertexesNumber(),
            49538);  // исправлена синтаксическая ошибка
}

TEST_F(ModelTest, CountVerticesAndFacets2) {
  model->LoadModel("obj/china.obj");
  EXPECT_EQ(model->GetFacetsNumber(), 114166);
  EXPECT_EQ(model->GetVertexesNumber(), 57085);
}

TEST_F(ModelTest, CountVerticesAndFacetsIncorrect) {
  EXPECT_THROW(model->LoadModel("obj/niheraaa.obj"), std::exception);
}

TEST_F(ModelTest, ParseModelDataIncorrect) {
  EXPECT_THROW(model->LoadModel("obj/vertex_id_out_of_range.obj"),
               std::exception);
}

TEST_F(ModelTest, ParseModelDataIncorrect2) {
  EXPECT_THROW(model->LoadModel("obj/vertex_with_less_than_3_coordinates.obj"),
               std::runtime_error);
}

TEST_F(ModelTest, ParseModelData) {
  model->LoadModel("obj/simple.obj");
  EXPECT_EQ(model->GetVertexesNumber(), 8);
  EXPECT_EQ(model->GetFacetsNumber(), 6);
  auto vertex = model->GetVertixes();
  auto facet = model->GetFacets();
  std::vector<std::vector<double>> vertex_orig = {
      {0, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
      {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}};
  std::vector<s21::Facet> facet_orig = {
      {std::vector<int>{1, 2, 3, 4}, 4}, {std::vector<int>{5, 6, 7, 8}, 4},
      {std::vector<int>{1, 2, 6, 5}, 4}, {std::vector<int>{2, 3, 7, 6}, 4},
      {std::vector<int>{3, 4, 8, 7}, 4}, {std::vector<int>{4, 1, 5, 8}, 4}};
  EXPECT_TRUE(vertex == vertex_orig);
  EXPECT_TRUE(facet == facet_orig);
}

TEST_F(ModelTest, CenterModelEmpty) {
  model->ClearModel();
  EXPECT_THROW(model->Centerise(), std::runtime_error);
}

TEST_F(ModelTest, ScaleModelToFitEmpty) {
  model->ClearModel();
  EXPECT_THROW(model->ScaleToFit(1.0), std::runtime_error);
}
}  // namespace s21

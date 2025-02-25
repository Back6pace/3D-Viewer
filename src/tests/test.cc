#include <gtest/gtest.h>

#include <filesystem>

#include "controllerFacade.h"
#define kEpsilon 1e-05

const std::string kCurrentPath = std::filesystem::current_path().string();

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Parser, FileNotFound) {
  s21::Parser parser;
  EXPECT_THROW(parser.ParseFile("test.obj"), std::runtime_error);
}

TEST(Parser, CubeTest) {
  s21::ModelBuilder model_builder;
  s21::ControllerFacade controller(model_builder);
  controller.ParseFile(kCurrentPath + "/tests/obj/cube_test.obj");
  EXPECT_EQ(controller.get_vertices().size() / 3, 8);
  EXPECT_EQ(controller.get_edges().size() / 2, 30);
}

TEST(Affine, translate_x) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::MoveX>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10 * 2 * 0.01);
  std::vector<double> vertices_e = {0.2, 0, 0, 0.2, 0, 2, 0.2, 2, 0, 0.2, 2, 2,
                                    2.2, 0, 0, 2.2, 0, 2, 2.2, 2, 0, 2.2, 2, 2};
  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, translate_y) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::MoveY>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10 * 2 * 0.01);
  std::vector<double> vertices_e = {0, 0.2, 0, 0, 0.2, 2, 0, 2.2, 0, 0, 2.2, 2,
                                    2, 0.2, 0, 2, 0.2, 2, 2, 2.2, 0, 2, 2.2, 2};
  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, translate_z) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::MoveZ>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10 * 2 * 0.01);
  std::vector<float> vertices_e = {0, 0, 0.2, 0, 0, 2.2, 0, 2, 0.2, 0, 2, 2.2,
                                   2, 0, 0.2, 2, 0, 2.2, 2, 2, 0.2, 2, 2, 2.2};
  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, rotate_x) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::RotateX>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10);

  std::vector<float> vertices_e = {0, 0,       0,        0, -0.347296, 1.96962,
                                   0, 1.96962, 0.347296, 0, 1.62232,   2.31691,
                                   2, 0,       0,        2, -0.347296, 1.96962,
                                   2, 1.96962, 0.347296, 2, 1.62232,   2.31691};

  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, rotate_y) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::RotateY>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10);

  std::vector<float> vertices_e = {0,       0, 0,         0.347296, 0, 1.96962,
                                   0,       2, 0,         0.347296, 2, 1.96962,
                                   1.96962, 0, -0.347296, 2.31691,  0, 1.62232,
                                   1.96962, 2, -0.347296, 2.31691,  2, 1.62232};

  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, rotate_z) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::RotateZ>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10);

  std::vector<float> vertices_e = {
      0,         0,       0, 0,       0,        2, -0.347296, 1.96962,  0,
      -0.347296, 1.96962, 2, 1.96962, 0.347296, 0, 1.96962,   0.347296, 2,
      1.62232,   2.31691, 0, 1.62232, 2.31691,  2};

  for (size_t i = 0; i < vertices_e.size(); ++i) {
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}

TEST(Affine, scale) {
  s21::Client affine_strategy_;
  std::unique_ptr<s21::AffineStrategy> strategy;
  strategy = std::make_unique<s21::Scale>();
  std::vector<float> vertices_ = {0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0, 2.0,
                                  0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 0.0, 2.0,
                                  0.0, 2.0, 2.0, 2.0, 0.0, 2.0, 2.0, 2.0};
  affine_strategy_.SetStrategy(std::move(strategy));
  affine_strategy_.Transform(vertices_, 10);

  std::vector<float> vertices_e = {0,  0, 0, 0,  0, 20, 0,  20, 0, 0,  20, 20,
                                   20, 0, 0, 20, 0, 20, 20, 20, 0, 20, 20, 20};

  for (size_t i = 0; i < vertices_e.size(); ++i) {
    // std::cout << vertices_[i] << " ,";
    // if ((i + 1) % 3 == 0) std::cout << std::endl;
    EXPECT_NEAR(vertices_[i], vertices_e[i], kEpsilon);
  }
}
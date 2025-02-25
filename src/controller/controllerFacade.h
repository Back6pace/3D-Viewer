#ifndef SRC_CONTROLLER_CONTROLLERFACADE_H
#define SRC_CONTROLLER_CONTROLLERFACADE_H

#include "affineStrategy.h"
#include "modelBuilder.h"
#include "parser.h"

namespace s21 {

class ControllerFacade {
 public:
  explicit ControllerFacade(ModelBuilder& model_builder)
      : model_builder_(model_builder) {}
  ~ControllerFacade() = default;

  // Фасад для загрузки файла
  void ParseFile(const std::string& file_path) {
    Parser parser;
    auto [vertices, edges] = parser.ParseFile(file_path);

    model_ = model_builder_.set_vertices(vertices).set_edges(edges).Build();
  }

  // Фасад для очистки данных
  void ClearModel() { model_ = Model(); }

  // Фасад для получения данных модели
  std::vector<float>& get_vertices() {
    if (settings::kDebugMode || settings::kDebugController) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "\nController get_vertices called, model address: "
                 << &model_ << std::endl;

      for (size_t i = 0; i < model_.get_vertices().size(); ++i) {
        debug_file << model_.get_vertices()[i] << " ";
        if ((i + 1) % 3 == 0) {
          debug_file << std::endl;
        }
      }
      if (model_.get_vertices().size() % 3 != 0) {
        debug_file << std::endl;
      }
      debug_file.close();
      std::cout << "Данные записаны в debug.log" << std::endl;
    }
    return model_.get_vertices();
  }
  std::vector<unsigned int>& get_edges() {
    // Debug
    if (settings::kDebugMode || settings::kDebugController) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "\nController get_edges called, model address: " << &model_
                 << std::endl;

      for (size_t i = 0; i < model_.get_edges().size(); i += 2) {
        debug_file << "Edge " << model_.get_edges()[i] << " "
                   << model_.get_edges()[i + 1] << std::endl;
      }

      debug_file.close();
      std::cout << "Данные записаны в debug.log" << std::endl;
    }
    return model_.get_edges();
  }

  std::size_t get_edges_count() {
    size_t edge_count = 0;
    edge_count = get_edges().size() / 2;
    return edge_count;
  }

  // Фасад для аффинных трансформаций
  void TransformModel(Mode mode, std::vector<float>& vertices, double k) {
    std::unique_ptr<AffineStrategy> strategy;
    std::string strategy_name;

    if (mode == kMoveX) {
      strategy = std::make_unique<MoveX>();
      strategy_name = "MoveX";
    } else if (mode == kMoveY) {
      strategy = std::make_unique<MoveY>();
      strategy_name = "MoveY";
    } else if (mode == kMoveZ) {
      strategy = std::make_unique<MoveZ>();
      strategy_name = "MoveZ";
    } else if (mode == kRotateX) {
      strategy = std::make_unique<RotateX>();
      strategy_name = "RotateX";
    } else if (mode == kRotateY) {
      strategy = std::make_unique<RotateY>();
      strategy_name = "RotateY";
    } else if (mode == kRotateZ) {
      strategy = std::make_unique<RotateZ>();
      strategy_name = "RotateZ";
    } else if (mode == kScale) {
      strategy = std::make_unique<Scale>();
    }

    if (settings::kDebugMode || settings::kDebugController) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "Selected strategy: " << strategy_name << "\n";
      debug_file.close();
    }

    if (strategy) {
      affine_strategy_.SetStrategy(std::move(strategy));
      affine_strategy_.Transform(vertices, k);
    }
  }

 private:
  ModelBuilder& model_builder_;
  Model model_;
  Client affine_strategy_;
};

}  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLERFACADE_H

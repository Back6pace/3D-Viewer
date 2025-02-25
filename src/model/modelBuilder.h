#ifndef SRC_MODEL_MODELBUILDER_H_
#define SRC_MODEL_MODELBUILDER_H_

#include <iostream>

#include "settings.h"

namespace s21 {

class Model {
 public:
  Model() = default;
  Model(std::vector<float> vertices, std::vector<unsigned int> edges)
      : vertices_(std::move(vertices)), edges_(std::move(edges)) {}

  std::vector<float>& get_vertices() {
    if (settings::kDebugMode || settings::kDebugModel) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "\nModelBuilder: get vertices, size: " << vertices_.size()
                 << std::endl;
      for (size_t i = 0; i < vertices_.size(); ++i) {
        debug_file << vertices_[i] << " ";

        if ((i + 1) % 3 == 0) {
          debug_file << std::endl;
        }
      }
      if (vertices_.size() % 3 != 0) {
        debug_file << std::endl;
      }

      debug_file.close();
      std::cout << "Information saved in debug.log(ModelBuilder getter)"
                << std::endl;
    }
    return vertices_;
  }
  std::vector<unsigned int>& get_edges() {
    if (settings::kDebugMode || settings::kDebugModel) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "\nModelBuilder: get edges, size: " << edges_.size()
                 << std::endl;

      for (size_t i = 0; i < edges_.size(); i += 2) {
        debug_file << "Edge " << edges_[i] << " " << edges_[i + 1] << std::endl;
      }

      debug_file.close();
      std::cout << "Information saved in debug.log(ModelBuilder getter)"
                << std::endl;
    }

    return edges_;
  }

 private:
  std::vector<float> vertices_;
  std::vector<unsigned int> edges_;
};

class ModelBuilder {
 public:
  ModelBuilder& set_vertices(const std::vector<float>& vertices) {
    vertices_ = vertices;

    if (settings::kDebugMode || settings::kDebugModel) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);
      debug_file << "\nModelBuilder: set vertices, size: " << vertices_.size()
                 << std::endl;
      for (size_t i = 0; i < vertices_.size(); ++i) {
        debug_file << vertices_[i] << " ";

        // После каждого третьего элемента делаем перевод строки
        if ((i + 1) % 3 == 0) {
          debug_file << std::endl;
        }
      }
      if (vertices_.size() % 3 != 0) {
        debug_file << std::endl;
      }

      debug_file.close();
      std::cout << "Information saved in debug.log(ModelBuilder setter)"
                << std::endl;
    }
    return *this;
  }

  ModelBuilder& set_edges(const std::vector<unsigned int>& edges) {
    edges_ = edges;

    if (settings::kDebugMode || settings::kDebugModel) {
      std::ofstream debug_file(settings::kDebugFile, std::ios::app);

      debug_file << "\nModelBuilder: set edges, size: " << edges_.size()
                 << std::endl;

      for (size_t i = 0; i < edges_.size(); i += 2) {
        debug_file << "Edge " << edges_[i] << " " << edges_[i + 1] << std::endl;
      }
      debug_file.close();
      std::cout << "Information saved in debug.log(ModelBuilder setter)"
                << std::endl;
    }

    return *this;
  }

  Model Build() { return Model(vertices_, edges_); }

 private:
  std::vector<float> vertices_;
  std::vector<unsigned int> edges_;
};

}  // namespace s21

#endif  // SRC_MODELBUILDER_H_
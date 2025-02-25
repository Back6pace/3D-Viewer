#include "affineStrategy.h"

namespace s21 {

void MoveX::TransformModel(std::vector<float> &vertices, const unsigned start,
                           const unsigned end, const float k) {
  for (unsigned i = start; k != 0 && i < end; i += 3) vertices[i] += k;
  if (settings::kDebugMode || settings::kDebugAffine) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\n MoveX done\n";
    debug_file << "Range: [" << start << " - " << end << "]\n";
    debug_file << "MoveX factor: " << k << "\n";
    debug_file.close();
  }
}
void MoveY::TransformModel(std::vector<float> &vertices, const unsigned start,
                           const unsigned end, const float k) {
  for (unsigned i = start + 1; k != 0 && i < end; i += 3) vertices[i] += k;
  if (settings::kDebugMode || settings::kDebugAffine) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\n MoveY done\n";
    debug_file << "Range: [" << start << " - " << end << "]\n";
    debug_file << "MoveY factor: " << k << "\n";
    debug_file.close();
  }
}

void MoveZ::TransformModel(std::vector<float> &vertices, const unsigned start,
                           const unsigned end, const float k) {
  for (unsigned i = start + 2; k != 0 && i < end; i += 3) vertices[i] += k;
  if (settings::kDebugMode || settings::kDebugAffine) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\n MoveZ done\n";
    debug_file << "Range: [" << start << " - " << end << "]\n";
    debug_file << "MoveZ factor: " << k << "\n";
    debug_file.close();
  }
}

void Scale::TransformModel(std::vector<float> &vertices, const unsigned start,
                           const unsigned end, const float k) {
  for (unsigned i = start; k != 0 && i < end; ++i) vertices[i] *= k;
  if (settings::kDebugMode || settings::kDebugAffine) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\n Scale done\n";
    debug_file << "Range: [" << start << " - " << end << "]\n";
    debug_file << "Scale factor: " << k << "\n";
    debug_file.close();
  }
}

void RotateX::TransformModel(std::vector<float> &vertices, const unsigned start,
                             const unsigned end, const float k) {
  float radians = k * M_PI / 180.0f;
  float cosK = std::cos(radians);
  float sinK = std::sin(radians);

  for (unsigned i = start; i < end; i += 3) {
    float y = vertices[i + 1];
    float z = vertices[i + 2];

    vertices[i + 1] = y * cosK - z * sinK;
    vertices[i + 2] = y * sinK + z * cosK;
  }
}

void RotateY::TransformModel(std::vector<float> &vertices, const unsigned start,
                             const unsigned end, const float k) {
  float radians = k * M_PI / 180.0f;
  float cosK = std::cos(radians);
  float sinK = std::sin(radians);

  for (unsigned i = start; i < end; i += 3) {
    float x = vertices[i];
    float z = vertices[i + 2];

    vertices[i] = x * cosK + z * sinK;
    vertices[i + 2] = -x * sinK + z * cosK;
  }
}

void RotateZ::TransformModel(std::vector<float> &vertices, const unsigned start,
                             const unsigned end, const float k) {
  float radians = k * M_PI / 180.0f;
  float cosK = std::cos(radians);
  float sinK = std::sin(radians);

  for (unsigned i = start; i < end; i += 3) {
    float x = vertices[i];
    float y = vertices[i + 1];

    vertices[i] = x * cosK - y * sinK;
    vertices[i + 1] = x * sinK + y * cosK;
  }
}

void Client::SetStrategy(std::unique_ptr<AffineStrategy> v) {
  operation = std::move(v);

  if (settings::kDebugMode || settings::kDebugAffine) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nClient::SetStrategy called\n";
    debug_file.close();
  }
}

void Client::Transform(std::vector<float> &vertices, const float k) {
  unsigned numbers_of_threads = std::thread::hardware_concurrency() - 1;

  if (numbers_of_threads == 0) numbers_of_threads = 1;

  unsigned start = vertices.size() / numbers_of_threads;
  while (start % 3 != 0) ++start;

  std::vector<std::thread> threads;
  for (unsigned i = 0; i < numbers_of_threads; ++i) {
    unsigned start_index = i * start;
    unsigned end_index =
        (i == numbers_of_threads - 1) ? vertices.size() : (i + 1) * start;

    threads.emplace_back([this, &vertices, start_index, end_index, k]() {
      operation->TransformModel(vertices, start_index, end_index, k);
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }
}
}  // namespace s21
#ifndef SRC_MODEL_AFFINE_TRANSFORMATIONS_AFFINE_STRATEGY_H_
#define SRC_MODEL_AFFINE_TRANSFORMATIONS_AFFINE_STRATEGY_H_

#include <cmath>
#include <thread>
#include <vector>

#include "settings.h"

namespace s21 {
enum Mode {
  kMoveX,
  kMoveY,
  kMoveZ,
  kRotateX,
  kRotateY,
  kRotateZ,
  kScale,
  kDefault
};
class AffineStrategy {
 public:
  virtual ~AffineStrategy() = default;
  virtual void TransformModel(std::vector<float> &vertices,
                              const unsigned start, const unsigned end,
                              const float k) = 0;
};

class MoveX : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class MoveY : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class MoveZ : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class RotateX : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class RotateY : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class RotateZ : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class Scale : public AffineStrategy {
 public:
  void TransformModel(std::vector<float> &vertices, const unsigned start,
                      const unsigned end, const float k);
};

class Context {
 public:
  virtual ~Context() = default;
  virtual void Transform(std::vector<float> &vertices, const float k) = 0;
  virtual void SetStrategy(std::unique_ptr<AffineStrategy> o) = 0;

 protected:
  std::unique_ptr<AffineStrategy> operation;
};

class Client : public Context {
 public:
  void SetStrategy(std::unique_ptr<AffineStrategy> v) override;
  void Transform(std::vector<float> &vertices, const float k) override;
};

}  // namespace s21

#endif  // SRC_MODEL_AFFINE_TRANSFORMATIONS_AFFINE_STRATEGY_H_
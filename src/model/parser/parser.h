#ifndef SRC_MODEL_PARSER_PARSER_H_
#define SRC_MODEL_PARSER_PARSER_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "settings.h"

namespace s21 {

class Parser {
 public:
  std::tuple<std::vector<float>, std::vector<unsigned int>> ParseFile(
      const std::string& file_path);
  size_t get_verteces_count() const { return verteces_count_; }

 private:
  std::size_t verteces_count_ = 0;
  bool ParsingObj(std::ifstream& file_obj, std::vector<float>& vertices,
                  std::vector<unsigned int>& edges);
  void RemoveExtraSpaces(std::string& str);
  void ParsingVertex(const std::string& line, std::vector<float>& vertices);
  void ParsingLine(const std::string& line, std::vector<unsigned int>& edges);
  void SplitEdgesToPairs(const std::vector<unsigned int>& line_indices,
                         std::vector<unsigned int>& edges);
};

}  // namespace s21

#endif  // SRC_MODEL_PARSER_PARSER_H_

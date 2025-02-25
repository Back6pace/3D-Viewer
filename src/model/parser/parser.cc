#include "parser.h"

namespace s21 {
std::tuple<std::vector<float>, std::vector<unsigned int>> Parser::ParseFile(
    const std::string &file_path) {
  std::ifstream f_open(file_path);
  std::vector<float> vertices;
  std::vector<unsigned int> edges;
  if (!f_open.is_open()) {
    throw std::runtime_error("Ошибка: не удалось открыть файл " + file_path);
  }

  Parser::ParsingObj(f_open, vertices, edges);

  return {std::move(vertices), std::move(edges)};
}

bool Parser::ParsingObj(std::ifstream &file_obj, std::vector<float> &vertices,
                        std::vector<unsigned int> &edges) {
  std::string line;

  while (std::getline(file_obj, line)) {
    if (!line.empty()) {
      RemoveExtraSpaces(line);

      if (line.compare(0, 2, "v ") == 0) {
        ParsingVertex(line, vertices);
      } else if (line.compare(0, 2, "f ") == 0) {
        ParsingLine(line, edges);
      }
    }
  }

  if (settings::kDebugMode || settings::kDebugParser) {
    std::ofstream debug_file(settings::kDebugFile, std::ios::app);
    debug_file << "\nParser debug, vertices: " << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
      debug_file << vertices[i] << " ";

      if ((i + 1) % 3 == 0) {
        debug_file << std::endl;
      }
    }
    if (vertices.size() % 3 != 0) {
      debug_file << std::endl;
    }
    debug_file << "Parser debug, edges:" << std::endl;
    for (size_t i = 0; i < edges.size(); i += 2) {
      debug_file << "Edge " << edges[i] << " " << edges[i + 1] << std::endl;
    }

    debug_file.close();
    std::cout << "Information saved in debug.log(Parser)" << std::endl;
  }

  return true;
}

void Parser::RemoveExtraSpaces(std::string &str) {
  size_t len = str.length();
  size_t write_idx = 0;
  bool space = false;

  for (size_t read_idx = 0; read_idx < len; ++read_idx) {
    if (str[read_idx] != ' ') {
      str[write_idx++] = str[read_idx];
      space = false;
    } else if (!space) {
      str[write_idx++] = ' ';
      space = true;
    }
  }

  str.resize(write_idx);
}

void Parser::ParsingVertex(const std::string &line,
                           std::vector<float> &vertices) {
  const char *str = line.c_str();
  while (*str && *str != ' ') ++str;

  while (*str) {
    char *end;
    float value = std::strtof(str, &end);
    if (str == end) break;
    vertices.push_back(value);
    verteces_count_++;
    str = end;
  }
}

void Parser::ParsingLine(const std::string &line,
                         std::vector<unsigned int> &edges) {
  const char *str = line.c_str();
  while (*str && *str != ' ') ++str;

  unsigned int first_index;
  bool is_first_index = true;
  std::vector<unsigned int> line_indices;

  while (*str) {
    char *end;
    long index = std::strtol(str, &end, 10);
    if (str == end) break;

    while (*end == '/' || std::isdigit(*end)) {
      ++end;
    }

    if (index < 0) {
      index += verteces_count_ / 3;
    } else {
      index -= 1;
    }

    if (index >= 0) {
      if (is_first_index) {
        first_index = static_cast<unsigned int>(index);
        is_first_index = false;
      }
      line_indices.push_back(static_cast<unsigned int>(index));
    }

    str = end;
    while (*str == ' ') ++str;
  }

  line_indices.push_back(first_index);
  SplitEdgesToPairs(line_indices, edges);
}

void Parser::SplitEdgesToPairs(const std::vector<unsigned int> &line_indices,
                               std::vector<unsigned int> &edges) {
  if (line_indices.size() < 2) return;

  for (size_t i = 0; i < line_indices.size() - 1; ++i) {
    edges.push_back(line_indices[i]);
    edges.push_back(line_indices[i + 1]);
  }
}

}  // namespace s21
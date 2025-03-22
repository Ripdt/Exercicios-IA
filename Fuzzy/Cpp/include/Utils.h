#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils
{
  std::vector<std::vector<std::string>> readCSV(
    const std::string &filename
  );

  std::vector<std::string> split(
    const std::string& s,
    const char delimiter
  );

  void rightPadTo(
    std::string& str,
    const size_t num,
    const char paddingChar = ' '
  );

  void leftPadTo(
    std::string& str,
    const size_t num,
    const char paddingChar = ' '
  );
};

#endif
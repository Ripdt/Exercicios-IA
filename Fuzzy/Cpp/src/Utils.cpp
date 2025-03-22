#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::vector<std::string>> Utils::readCSV(
    const std::string &filename
)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line))
    {
      std::vector<std::string> row = split(line, ';');

        data.push_back(row);
    }

    file.close();
    return data;
}

std::vector<std::string> Utils::split(
  const std::string& s,
  const char delimiter
)
{
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

void Utils::rightPadTo(
  std::string& str,
  const size_t num,
  const char paddingChar
)
{
  if (num > str.size())
    str.insert(0, num - str.size(), paddingChar);
}

void Utils::leftPadTo(
  std::string& str,
  const size_t num,
  const char paddingChar
)
{
  if (num > str.size())
    str.insert(str.size(), num - str.size(), paddingChar);
}
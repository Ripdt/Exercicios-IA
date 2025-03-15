#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils
{
    std::vector<std::vector<std::string>> readCSV(
        const std::string &filename);
};

#endif
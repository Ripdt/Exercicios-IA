#include "Fuzzyficator.h"

#include <algorithm>

void Fuzzyficator::ruleAND(
    std::unordered_map<std::string, float> &variables,
    const std::string &var1,
    const std::string &var2,
    const std::string &varr) const
{
    const float value = std::min(variables[var1], variables[var2]);
    variables[varr] = variables.find(varr) != variables.end() ? std::max(value, variables[varr]) : value;
}

void Fuzzyficator::ruleOR(
    std::unordered_map<std::string, float> &variables,
    const std::string &var1,
    const std::string &var2,
    const std::string &varr) const
{
    const float value = std::max(variables[var1], variables[var2]);
    variables[varr] = variables.find(varr) != variables.end() ? std::max(value, variables[varr]) : value;
}

#include "VariableGroup.h"

void VariableGroup::add(
    const FuzzyVariable &var)
{
    vars.push_back(var);
}

void VariableGroup::fuzzyfication(
    float value,
    std::unordered_map<std::string, float> &fuzzyVariables)
{
    for (auto &var : vars)
    {
        fuzzyVariables[var.getNome()] = var.fuzzyfication(value);
    }
}
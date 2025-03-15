#ifndef VARIABLE_GROUP_H
#define VARIABLE_GROUP_H

#include <list>
#include <unordered_map>
#include <string>

#include "FuzzyVariable.h"

class VariableGroup
{
private:
    std::list<FuzzyVariable> vars;

public:
    VariableGroup() = default;

    void add(const FuzzyVariable &var);

    void fuzzyfication(const float value, std::unordered_map<std::string, float> &fuzzyVariables);
};

#endif
#ifndef FUZZYFICATOR_H
#define FUZZYFICATOR_H

#include <string>
#include <unordered_map>

class Fuzzyficator
{
public:
    Fuzzyficator() = default;

    virtual void fuzzyfication() const = 0;

protected:
    void ruleAND(
        std::unordered_map<std::string, float> &variables,
        const std::string &var1,
        const std::string &var2,
        const std::string &varr) const;

    void ruleOR(
        std::unordered_map<std::string, float> &variables,
        const std::string &var1,
        const std::string &var2,
        const std::string &varr) const;
};

#endif
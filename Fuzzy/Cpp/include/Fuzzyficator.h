#ifndef FUZZYFICATOR_H
#define FUZZYFICATOR_H

#include <string>
#include <unordered_map>

class Result
{
public:
  Result(
    const std::string& _name,
    const float _score,
    const float _nonAtractiveScore,
    const float _atractiveScore,
    const float _veryAtractiveScore
  );

  bool operator<(
    const Result& other
    ) const;

  std::string name;
  float score, nonAtractiveScore, atractiveScore, veryAtractiveScore;
};

class Fuzzyficator
{
public:
    Fuzzyficator() = default;

    void fuzzyfication();

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

    std::vector<Result> results;

private:
  virtual void readSpreadsheet() = 0;

  void printResults();
};

#endif
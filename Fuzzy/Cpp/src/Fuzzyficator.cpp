#include "Fuzzyficator.h"

#include "TablePrinter.h"

#include <algorithm>
#include <iostream>

// ----------------------------------------------------------------------------
// Result
// ----------------------------------------------------------------------------

Result::Result(
  const std::string& _name,
  const float _score,
  const float _nonAtractiveScore,
  const float _atractiveScore,
  const float _veryAtractiveScore
) :
  name(_name),
  score(_score),
  nonAtractiveScore(_nonAtractiveScore),
  atractiveScore(_atractiveScore),
  veryAtractiveScore(_veryAtractiveScore)
{
}

bool Result::operator<(const Result& other) const
{
  return score < other.score || score == other.score && (
    veryAtractiveScore < other.veryAtractiveScore || veryAtractiveScore == other.veryAtractiveScore && (
      atractiveScore < other.atractiveScore || atractiveScore == other.atractiveScore && (
        nonAtractiveScore < other.nonAtractiveScore || nonAtractiveScore == other.nonAtractiveScore && (
          name < other.name
          )
        )
      )
    );
}

// ----------------------------------------------------------------------------
// Fuzzyficator
// ----------------------------------------------------------------------------

void Fuzzyficator::fuzzyfication()
{
  readSpreadsheet();
  printResults();
}

void Fuzzyficator::printResults()
{
  std::sort(results.begin(), results.end(), [](const Result& mr1, const Result& mr2) { return mr2 < mr1; });

  std::cout << "\n\n";
  TablePrinter resultsTable({ "Name", "NA", "A", "VA", "Score" }, { 32, 9, 9, 9, 9 });
  resultsTable.printHeader();

  //for (size_t i = 0; i != 50; i++) {
  //  const Result& result = results[i];
  //  resultsTable.printRow(result.name, result.nonAtractiveScore, result.atractiveScore, result.veryAtractiveScore, result.score);
  //}
  //for (const Result& result : results) {
  //  if (result.score != 10) break;
  //  resultsTable.printRow(result.name, result.nonAtractiveScore, result.atractiveScore, result.veryAtractiveScore, result.score);
  //}

  //  resultsTable.printRow(result.name, result.nonAtractiveScore, result.atractiveScore, result.veryAtractiveScore, result.score);
  //}
  for (const Result& result : results)
    resultsTable.printRow(result.name, result.nonAtractiveScore, result.atractiveScore, result.veryAtractiveScore, result.score);
}

void Fuzzyficator::ruleAND(
    std::unordered_map<std::string, float> &variables,
    const std::string &var1,
    const std::string &var2,
    const std::string &varr
) const
{
  const float value = std::min(variables[var1], variables[var2]);
  variables[varr] = variables.find(varr) != variables.end() ? std::max(value, variables[varr]) : value;
}

void Fuzzyficator::ruleOR(
    std::unordered_map<std::string, float> &variables,
    const std::string &var1,
    const std::string &var2,
    const std::string &varr
) const
{
    const float value = std::max(variables[var1], variables[var2]);
    variables[varr] = variables.find(varr) != variables.end() ? std::max(value, variables[varr]) : value;
}
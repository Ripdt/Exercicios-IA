#ifndef FUZZY_VARIABLE_H
#define FUZZY_VARIABLE_H

#include <string>

class FuzzyVariable
{
private:
  std::string nome;
  float bottom1, top1, top2, bottom2;

public:
  FuzzyVariable(
      const std::string &_nome,
      const float _bottom1,
      const float _top1,
      const float _top2,
      const float _bottom2);

  ~FuzzyVariable() = default;

  float fuzzyfication(
      const float value) const;

  std::string getNome() const;
};

#endif
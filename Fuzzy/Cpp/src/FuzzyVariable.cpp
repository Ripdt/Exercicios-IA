#include "FuzzyVariable.h"

FuzzyVariable::FuzzyVariable(
    const std::string &_nome,
    const float _bottom1,
    const float _top1,
    const float _top2,
    const float _bottom2) : nome(_nome),
                            bottom1(_bottom1),
                            top1(_top1),
                            top2(_top2),
                            bottom2(_bottom2)
{
}

float FuzzyVariable::fuzzyfication(
    const float value) const
{
    if (value < bottom1 || value > bottom2)
    {
        return 0;
    }

    if (value >= top1 && value <= top2)
    {
        return 1;
    }

    if (value > bottom1 && value < top1)
    {
        float p = (value - bottom1) / (top1 - bottom1);
        return p;
    }

    if (value > top2 && value < bottom2)
    {
        float p = 1.0f - ((value - top2) / (bottom2 - top2));
        return p;
    }

    return 0;
}

std::string FuzzyVariable::getNome() const
{
    return nome;
}
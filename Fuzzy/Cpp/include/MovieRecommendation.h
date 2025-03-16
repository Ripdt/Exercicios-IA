#ifndef MOVIE_RECOMMENDATION_H
#define MOVIE_RECOMMENDATION_H

#include "Fuzzyficator.h"

class MovieRecommendation : public Fuzzyficator
{
public:
    MovieRecommendation() = default;
    ~MovieRecommendation() = default;

    void fuzzyfication() const override;
};

#endif
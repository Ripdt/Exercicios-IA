#ifndef MOVIE_RECOMMENDATION_H
#define MOVIE_RECOMMENDATION_H

#include "Fuzzyficator.h"
#include "VariableGroup.h"

class MovieRecommendation : public Fuzzyficator
{
public:
    MovieRecommendation() = default;
    ~MovieRecommendation() = default;

private:
    void readSpreadsheet() override;

    void initializeGroups();

    void applyRules(
      std::unordered_map<std::string, float>& variables
    ) const;

    VariableGroup votesQuantityGroup, ratingGroup, popularityGroup, releaseYearGroup, genreGroup;
};

#endif
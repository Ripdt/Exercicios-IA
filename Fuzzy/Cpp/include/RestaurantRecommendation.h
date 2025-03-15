#ifndef RESTAURANT_RECOMMENDATION_H
#define RESTAURANT_RECOMMENDATION_H

#include "Fuzzyficator.h"

class RestaurantRecommendation : public Fuzzyficator
{
public:
    RestaurantRecommendation() = default;

    void fuzzyfication() const override;
};

#endif
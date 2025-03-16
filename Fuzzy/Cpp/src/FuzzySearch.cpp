#include "RestaurantRecommendation.h"
#include "MovieRecommendation.h"

#include <iostream>

int main()
{
  std::cout << "Select what you want to do:" << std::endl;
  std::cout << "1. Restaurant Recommendation" << std::endl;
  std::cout << "2. Movie Recommendation" << std::endl;

  int option;
  std::cin >> option;

  if (option == 1) {
    RestaurantRecommendation recommendation;
    recommendation.fuzzyfication();
  }
  else if (option == 2) {
    MovieRecommendation recommendation;
    recommendation.fuzzyfication();
  }
  else {
    std::cout << "Invalid option" << std::endl;
  }
  return 0;
}
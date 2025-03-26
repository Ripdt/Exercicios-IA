#include "MovieRecommendation.h"

#include "TablePrinter.h"
#include "Utils.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

void MovieRecommendation::readSpreadsheet()
{
  initializeGroups();

  std::unordered_map<std::string, float> genresPreference = {
      {"Crime", 10}, 
      {"Thriller", 9.5f},
      {"Drama", 9},
      {"Action", 8.5f},
      {"Fantasy", 8},
      {"Horror", 7.5f},
      {"Foreign", 7}, 
      {"Mystery", 6.5f},
      {"Animation", 6}, 
      {"Music", 5.5f}, 
      {"Family", 5}, 
      {"War", 4.5f},
      {"Comedy", 4}, 
      {"Romance", 3.5f}, 
      {"History", 3}, 
      {"Adventure", 2.5f},
      {"Documentary", 2}, 
      {"Science", 1.5f}, 
      {"Western", 1}, 
      {"TV", 0.5f}
  };

  try
  {
    TablePrinter movieTable(
      { "Name", "Year", "Popularity", "Duration", "Rating", "VotesQtd", "Genres"},
      { 23, 5, 11, 9, 9, 9, 23}
    );
    movieTable.printHeader();

    typedef std::vector<std::vector<std::string>> Spreadsheet;
    const Spreadsheet &spreadsheet = Utils::readCSV("../../movie_dataset.csv");
    for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
    {
      const std::vector<std::string> &line = *it;
      if (line.size() <= 20) continue;

      float popularity, runtime, rating, qtdVotes, firstGenrePreference, secondGenrePreference;
      int year;
      bool hasSecondGenre = false, hasThirdGenre = false;

      try {
        std::string name = line[18].substr(0, 23);
        Utils::leftPadTo(name, 23);

        const std::vector<std::string> genres = Utils::split(line[2], ' ');

        if (genres.size() == 0) continue;

        hasSecondGenre = genres.size() > 1;

        firstGenrePreference  = genresPreference[genres[0]];
        if (hasSecondGenre) {
          secondGenrePreference = genresPreference[genres[1]];
        }
        popularity = std::stof(line[9].substr(0, 6).c_str());
        runtime = std::stof(line[14].c_str());
        rating = std::stof(line[19].c_str());
        qtdVotes = std::stof(line[20].c_str());

        const std::string releaseDate = line[12].substr(0, 4);
        year = std::stoi(releaseDate);

        movieTable.printRow(name, year, popularity, runtime, rating, qtdVotes, line[2]);

        if (line[18] == "Reindeer Games") {
          const int a = 1 + 1;
        }
      }
      catch (...) {
        continue;
      }

      std::unordered_map<std::string, float> variables;

      votesQuantityGroup.fuzzyfication(qtdVotes, variables);
      runtimeGroup.fuzzyfication(runtime, variables);
      ratingGroup.fuzzyfication(rating, variables);
      popularityGroup.fuzzyfication(popularity, variables);
      releaseYearGroup.fuzzyfication(year, variables);
      firstGenreGroup.fuzzyfication(firstGenrePreference, variables);
      if (hasSecondGenre)
        secondGenreGroup.fuzzyfication(secondGenrePreference, variables);

      applyRules(variables, hasSecondGenre);

      const float NA = variables["NA"];
      const float A = variables["A"];
      const float MA = variables["VA"];

      const float totalScore = NA + A + MA;
      const float score = totalScore == 0 ? 0 : (NA * 1.0f + A * 5.0f + MA * 10.0f) / totalScore;

      const Result result(line[18], score, NA, A, MA);
      results.push_back(result);
    }
  }
  catch (...)
  {
    std::cout << "\n\nErro ao ler arquivo\n\n";
  }
}

void MovieRecommendation::initializeGroups()
{
  votesQuantityGroup.add(FuzzyVariable("Very Low - QtdVotes", 0, 0, 50, 100));
  votesQuantityGroup.add(FuzzyVariable("Low - QtdVotes", 75, 120, 150, 200));
  votesQuantityGroup.add(FuzzyVariable("Medium - QtdVotes", 175, 200, 300, 400));
  votesQuantityGroup.add(FuzzyVariable("High - QtdVotes", 350, 1000, 2000, 3000));
  votesQuantityGroup.add(FuzzyVariable("Very High - QtdVotes", 2500, 6000, 14000, 14000));

  ratingGroup.add(FuzzyVariable("Very Bad - Rating", 0.0f, 0.0f, 1.0f, 3.0f));
  ratingGroup.add(FuzzyVariable("Bad - Rating", 2.0f, 3.0f, 4.0f, 5.0f));
  ratingGroup.add(FuzzyVariable("Medium - Rating", 4.5f, 5.0f, 5.5f, 6.5f));
  ratingGroup.add(FuzzyVariable("Good - Rating", 6.0f, 6.5f, 7.0f, 8.5f));
  ratingGroup.add(FuzzyVariable("Very Good - Rating", 8.0f, 8.2f, 9.0f, 9.5f));
  ratingGroup.add(FuzzyVariable("Excelent - Rating", 9.0f, 9.5f, 10.0f, 10.0f));

  runtimeGroup.add(FuzzyVariable("Very Short - Runtime", 0, 0, 20, 50));
  runtimeGroup.add(FuzzyVariable("Short - Runtime", 40, 55, 60, 75));
  runtimeGroup.add(FuzzyVariable("Medium - Runtime", 70, 80, 110, 130));
  runtimeGroup.add(FuzzyVariable("Long - Runtime", 120, 130, 150, 160));
  runtimeGroup.add(FuzzyVariable("Very Long - Runtime", 155, 170, 190, 190));

  popularityGroup.add(FuzzyVariable("Very Low - Popularity", 0, 0, 10, 20));
  popularityGroup.add(FuzzyVariable("Low - Popularity", 15, 20, 30, 40));
  popularityGroup.add(FuzzyVariable("Medium - Popularity", 35, 40, 50, 60));
  popularityGroup.add(FuzzyVariable("High - Popularity", 55, 60, 70, 80));
  popularityGroup.add(FuzzyVariable("Very High - Popularity", 75, 80, 90, 100));

  firstGenreGroup.add(FuzzyVariable("Very Low Interest - 1st Genre", 0, 0, 1.5, 2));
  firstGenreGroup.add(FuzzyVariable("Low Interest - 1st Genre", 1.5, 2, 2.5, 4));
  firstGenreGroup.add(FuzzyVariable("Medium Interest - 1st Genre", 3.5, 4, 6, 7));
  firstGenreGroup.add(FuzzyVariable("High Interest - 1st Genre", 6.5, 7, 8, 9));
  firstGenreGroup.add(FuzzyVariable("Very High Interest - 1st Genre", 8.5, 9.5, 10, 10));

  secondGenreGroup.add(FuzzyVariable("Very Low Interest - 2nd Genre", 0, 0, 1.5, 2));
  secondGenreGroup.add(FuzzyVariable("Low Interest - 2nd Genre", 1, 2, 2.5, 4));
  secondGenreGroup.add(FuzzyVariable("Medium Interest - 2nd Genre", 3.5, 4, 6, 7));
  secondGenreGroup.add(FuzzyVariable("High Interest - 2nd Genre", 6.5, 7, 8, 9));
  secondGenreGroup.add(FuzzyVariable("Very High Interest - 2nd Genre", 8.5, 9.5, 10, 10));

  releaseYearGroup.add(FuzzyVariable("Very Old - Release Year", 1900, 1900, 1950, 1960));
  releaseYearGroup.add(FuzzyVariable("Old - Release Year", 1950, 1960, 1970, 1980));
  releaseYearGroup.add(FuzzyVariable("Medium - Release Year", 1975, 1995, 2005, 2015));
  releaseYearGroup.add(FuzzyVariable("Recent - Release Year", 2010, 2015, 2020, 2025));
  releaseYearGroup.add(FuzzyVariable("Very Recent - Release Year", 2020, 2025, 2030, 2030));
}

void MovieRecommendation::applyRules(
  std::unordered_map<std::string, float>& variables,
  const bool hasSecondGenre
) const
{  
  // Release Year X QtdVotes
  ruleAND(variables, "Very Old - Release Year", "Very Low - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Very Old - Release Year", "Low - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Very Old - Release Year", "Medium - QtdVotes", "Votes_High");
  ruleAND(variables, "Very Old - Release Year", "High - QtdVotes", "Votes_VeryHigh");
  ruleAND(variables, "Very Old - Release Year", "Very High - QtdVotes", "Votes_VeryHigh");

  ruleAND(variables, "Old - Release Year", "Very Low - QtdVotes", "Votes_Low");
  ruleAND(variables, "Old - Release Year", "Low - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Old - Release Year", "Medium - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Old - Release Year", "High - QtdVotes", "Votes_High");
  ruleAND(variables, "Old - Release Year", "Very High - QtdVotes", "Votes_VeryHigh");

  ruleAND(variables, "Medium - Release Year", "Very Low - QtdVotes", "Votes_Low");
  ruleAND(variables, "Medium - Release Year", "Low - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Medium - Release Year", "Medium - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Medium - Release Year", "High - QtdVotes", "Votes_High");
  ruleAND(variables, "Medium - Release Year", "Very High - QtdVotes", "Votes_VeryHigh");

  ruleAND(variables, "Recent - Release Year", "Very Low - QtdVotes", "Votes_VeryLow");
  ruleAND(variables, "Recent - Release Year", "Low - QtdVotes", "Votes_Low");
  ruleAND(variables, "Recent - Release Year", "Medium - QtdVotes", "Votes_Medium");
  ruleAND(variables, "Recent - Release Year", "High - QtdVotes", "Votes_High");
  ruleAND(variables, "Recent - Release Year", "Very High - QtdVotes", "Votes_VeryHigh");

  ruleAND(variables, "New - Release Year", "Very Low - QtdVotes", "Votes_VeryLow");
  ruleAND(variables, "New - Release Year", "Low - QtdVotes", "Votes_VeryLow");
  ruleAND(variables, "New - Release Year", "Medium - QtdVotes", "Votes_Medium");
  ruleAND(variables, "New - Release Year", "High - QtdVotes", "Votes_High");
  ruleAND(variables, "New - Release Year", "Very High - QtdVotes", "Votes_High");

  // Rating X QtdVotes
  ruleAND(variables, "Very Low - Rating", "Votes_VeryLow", "Rating_NA");
  ruleAND(variables, "Very Low - Rating", "Votes_Low", "Rating_NA");
  ruleAND(variables, "Very Low - Rating", "Votes_Medium", "Rating_NA");
  ruleAND(variables, "Very Low - Rating", "Votes_High", "Rating_NA");
  ruleAND(variables, "Very Low - Rating", "Votes_VeryHigh", "Rating_NA");

  ruleAND(variables, "Low - Rating", "Votes_VeryLow", "Rating_NA");
  ruleAND(variables, "Low - Rating", "Votes_Low", "Rating_NA");
  ruleAND(variables, "Low - Rating", "Votes_Medium", "Rating_NA");
  ruleAND(variables, "Low - Rating", "Votes_High", "Rating_NA");
  ruleAND(variables, "Low - Rating", "Votes_VeryHigh", "Rating_NA");

  ruleAND(variables, "Medium - Rating", "Votes_VeryLow", "Rating_NA");
  ruleAND(variables, "Medium - Rating", "Votes_Low", "Rating_NA");
  ruleAND(variables, "Medium - Rating", "Votes_Medium", "Rating_NA");
  ruleAND(variables, "Medium - Rating", "Votes_High", "Rating_A");
  ruleAND(variables, "Medium - Rating", "Votes_VeryHigh", "Rating_A");

  ruleAND(variables, "Good - Rating", "Votes_VeryLow", "Rating_A");
  ruleAND(variables, "Good - Rating", "Votes_Low", "Rating_A");
  ruleAND(variables, "Good - Rating", "Votes_Medium", "Rating_A");
  ruleAND(variables, "Good - Rating", "Votes_High", "Rating_A");
  ruleAND(variables, "Good - Rating", "Votes_VeryHigh", "Rating_A");

  ruleAND(variables, "Very Good - Rating", "Votes_VeryLow", "Rating_A");
  ruleAND(variables, "Very Good - Rating", "Votes_Low", "Rating_A");
  ruleAND(variables, "Very Good - Rating", "Votes_Medium", "Rating_A");
  ruleAND(variables, "Very Good - Rating", "Votes_High", "Rating_A");
  ruleAND(variables, "Very Good - Rating", "Votes_VeryHigh", "Rating_VA");

  ruleAND(variables, "Excelent - Rating", "Votes_VeryLow", "Rating_A");
  ruleAND(variables, "Excelent - Rating", "Votes_Low", "Rating_A");
  ruleAND(variables, "Excelent - Rating", "Votes_Medium", "Rating_VA");
  ruleAND(variables, "Excelent - Rating", "Votes_High", "Rating_VA");
  ruleAND(variables, "Excelent - Rating", "Votes_VeryHigh", "Rating_VA");

  // Release Year X Popularity
  ruleAND(variables, "Very Old - Release Year", "Very Low - Popularity", "Pop_Low");
  ruleAND(variables, "Very Old - Release Year", "Low - Popularity", "Pop_Medium");
  ruleAND(variables, "Very Old - Release Year", "Medium - Popularity", "Pop_High");
  ruleAND(variables, "Very Old - Release Year", "High - Popularity", "Pop_VeryHigh");
  ruleAND(variables, "Very Old - Release Year", "Very High - Popularity", "Pop_VeryHigh");

  ruleAND(variables, "Old - Release Year", "Very Low - Popularity", "Pop_Low");
  ruleAND(variables, "Old - Release Year", "Low - Popularity", "Pop_Medium");
  ruleAND(variables, "Old - Release Year", "Medium - Popularity", "Pop_High");
  ruleAND(variables, "Old - Release Year", "High - Popularity", "Pop_VeryHigh");
  ruleAND(variables, "Old - Release Year", "Very High - Popularity", "Pop_VeryHigh");

  ruleAND(variables, "Medium - Release Year", "Very Low - Popularity", "Pop_Low");
  ruleAND(variables, "Medium - Release Year", "Low - Popularity", "Pop_Medium");
  ruleAND(variables, "Medium - Release Year", "Medium - Popularity", "Pop_Medium");
  ruleAND(variables, "Medium - Release Year", "High - Popularity", "Pop_High");
  ruleAND(variables, "Medium - Release Year", "Very High - Popularity", "Pop_VeryHigh");

  ruleAND(variables, "Recent - Release Year", "Very Low - Popularity", "Pop_VeryLow");
  ruleAND(variables, "Recent - Release Year", "Low - Popularity", "Pop_Low");
  ruleAND(variables, "Recent - Release Year", "Medium - Popularity", "Pop_Medium");
  ruleAND(variables, "Recent - Release Year", "High - Popularity", "Pop_High");
  ruleAND(variables, "Recent - Release Year", "Very High - Popularity", "Pop_VeryHigh");

  ruleAND(variables, "New - Release Year", "Very Low - Popularity", "Pop_VeryLow");
  ruleAND(variables, "New - Release Year", "Low - Popularity", "Pop_VeryLow");
  ruleAND(variables, "New - Release Year", "Medium - Popularity", "Pop_Low");
  ruleAND(variables, "New - Release Year", "High - Popularity", "Pop_Medium");
  ruleAND(variables, "New - Release Year", "Very High - Popularity", "Pop_High");

  // 1st Genre X 2nd Genre
  if (hasSecondGenre) {
    ruleAND(variables, "Very Low Interest - 1st Genre", "Very Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Very Low Interest - 1st Genre", "Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Very Low Interest - 1st Genre", "Medium Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Very Low Interest - 1st Genre", "High Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "Very Low Interest - 1st Genre", "Very High Interest - 2nd Genre", "Genre_A");

    ruleAND(variables, "Low Interest - 1st Genre", "Very Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Low Interest - 1st Genre", "Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Low Interest - 1st Genre", "Medium Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Low Interest - 1st Genre", "High Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "Low Interest - 1st Genre", "Very High Interest - 2nd Genre", "Genre_VA");

    ruleAND(variables, "Medium Interest - 1st Genre", "Very Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Medium Interest - 1st Genre", "Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Medium Interest - 1st Genre", "Medium Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "Medium Interest - 1st Genre", "High Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "Medium Interest - 1st Genre", "Very High Interest - 2nd Genre", "Genre_VA");

    ruleAND(variables, "High Interest - 1st Genre", "Very Low Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "High Interest - 1st Genre", "Low Interest - 2nd Genre", "Genre_A");
    ruleAND(variables, "High Interest - 1st Genre", "Medium Interest - 2nd Genre", "Genre_A");

    ruleAND(variables, "Very High Interest - 1st Genre", "Very Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Very High Interest - 1st Genre", "Low Interest - 2nd Genre", "Genre_NA");
    ruleAND(variables, "Very High Interest - 1st Genre", "Medium Interest - 2nd Genre", "Genre_NA");

    ruleOR(variables, "High Interest - 1st Genre", "High Interest - 2nd Genre", "Genre_VA");
    ruleOR(variables, "High Interest - 1st Genre", "Very High Interest - 2nd Genre", "Genre_VA");
    ruleOR(variables, "Very High Interest - 1st Genre", "High Interest - 2nd Genre", "Genre_VA");
    ruleOR(variables, "Very High Interest - 1st Genre", "Very High Interest - 2nd Genre", "Genre_VA");

    // Gender Atractiveness X Rating Atractiveness
    ruleOR(variables, "Gender_NA", "Rating_NA", "NA");
    ruleAND(variables, "Gender_NA", "Rating_A", "NA");
    ruleAND(variables, "Gender_NA", "Rating_VA", "NA");

    ruleAND(variables, "Gender_A", "Rating_NA", "NA");
    ruleOR(variables, "Gender_A", "Rating_A", "A");
    ruleAND(variables, "Gender_A", "Rating_VA", "A");

    ruleAND(variables, "Gender_VA", "Rating_NA", "NA");
    ruleOR(variables, "Gender_VA", "Rating_A", "A");
    ruleOR(variables, "Gender_VA", "Rating_VA", "VA");
  }
  else {
    ruleAND(variables, "Very Low Interest - 1st Genre", "Rating_NA", "NA");
    ruleAND(variables, "Very Low Interest - 1st Genre", "Rating_A", "NA");
    ruleAND(variables, "Very Low Interest - 1st Genre", "Rating_VA", "NA");

    ruleAND(variables, "Low Interest - 1st Genre", "Rating_NA", "NA");
    ruleAND(variables, "Low Interest - 1st Genre", "Rating_A", "NA");
    ruleAND(variables, "Low Interest - 1st Genre", "Rating_VA", "NA");

    ruleAND(variables, "Medium Interest - 1st Genre", "Rating_NA", "NA");
    ruleAND(variables, "Medium Interest - 1st Genre", "Rating_A", "NA");
    ruleAND(variables, "Medium Interest - 1st Genre", "Rating_VA", "A");

    ruleAND(variables, "High Interest - 1st Genre", "Rating_NA", "A");
    ruleOR(variables, "High Interest - 1st Genre", "Rating_A", "A");
    ruleOR(variables, "High Interest - 1st Genre", "Rating_VA", "VA");

    ruleAND(variables, "Very High Interest - 1st Genre", "Rating_NA", "NA");
    ruleOR(variables, "Very High Interest - 1st Genre", "Rating_A", "A");
    ruleOR(variables, "Very High Interest - 1st Genre", "Rating_VA", "VA");
  }

  // Popularity X Atractiveness
  //ruleAND(variables, "Pop_VeryLow", "NA", "NA");
  //ruleAND(variables, "Pop_Low", "NA", "NA");
  //ruleAND(variables, "Pop_Medium", "NA", "NA");
  //ruleAND(variables, "Pop_High", "NA", "NA");
  //ruleAND(variables, "Pop_VeryHigh", "NA", "NA");

  //ruleAND(variables, "Pop_VeryLow", "A", "A");
  //ruleAND(variables, "Pop_Low", "A", "A");
  //ruleAND(variables, "Pop_Medium", "A", "A");
  //ruleAND(variables, "Pop_High", "A", "A");
  //ruleAND(variables, "Pop_VeryHigh", "A", "A");

  //ruleAND(variables, "Pop_VeryLow", "VA", "VA");
  //ruleAND(variables, "Pop_Low", "VA", "VA");
  //ruleAND(variables, "Pop_Medium", "VA", "VA");
  //ruleAND(variables, "Pop_High", "VA", "VA");
  //ruleAND(variables, "Pop_VeryHigh", "VA", "VA");

  // Atractiveness X Runtime
  //ruleAND(variables, "NA", "Very Short - Runtime", "NA");
  //ruleAND(variables, "NA", "Short - Runtime", "NA");
  //ruleAND(variables, "NA", "Medium - Runtime", "NA");
  //ruleAND(variables, "NA", "Long - Runtime", "NA");
  //ruleAND(variables, "NA", "Very Long - Runtime", "NA");

  //ruleAND(variables, "A", "Very Short - Runtime", "A");
  //ruleAND(variables, "A", "Short - Runtime", "A");
  //ruleAND(variables, "A", "Medium - Runtime", "A");
  //ruleAND(variables, "A", "Long - Runtime", "NA");
  ruleOR(variables, "A", "Very Long - Runtime", "NA");

  //ruleAND(variables, "VA", "Very Short - Runtime", "A");
  //ruleAND(variables, "VA", "Short - Runtime", "VA");
  //ruleAND(variables, "VA", "Medium - Runtime", "VA");
  //ruleAND(variables, "VA", "Long - Runtime", "A");
  ruleOR(variables, "VA", "Very Long - Runtime", "NA");
}
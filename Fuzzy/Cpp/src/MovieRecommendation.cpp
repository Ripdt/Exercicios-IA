#include "MovieRecommendation.h"

#include "TablePrinter.h"
#include "Utils.h"
#include "VariableGroup.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

struct MovieResult
{
  std::string name;
  float score, nonAtractiveScore, atractiveScore, veryAtractiveScore;
};

void MovieRecommendation::initializeGroups()
{
  votesQuantityGroup.add(FuzzyVariable("Very Low - QtdVotes", 0, 0, 100, 200));
  votesQuantityGroup.add(FuzzyVariable("Low - QtdVotes", 150, 200, 250, 400));
  votesQuantityGroup.add(FuzzyVariable("Medium - QtdVotes", 300, 400, 500, 800));
  votesQuantityGroup.add(FuzzyVariable("High - QtdVotes", 700, 1000, 2000, 4000));
  votesQuantityGroup.add(FuzzyVariable("Very High - QtdVotes", 3500, 6000, 10000, 13000));

  ratingGroup.add(FuzzyVariable("Very Bad - Rating", 0.0f, 0.0f, 1.0f, 3.0f));
  ratingGroup.add(FuzzyVariable("Bad - Rating", 2.0f, 3.0f, 4.0f, 5.0f));
  ratingGroup.add(FuzzyVariable("Medium - Rating", 4.5f, 5.0f, 5.5f, 6.0f));
  ratingGroup.add(FuzzyVariable("Good - Rating", 5.5f, 6.5f, 7.0f, 8.0f));
  ratingGroup.add(FuzzyVariable("Very Good - Rating", 7.5f, 8.2f, 9.0f, 9.5f));
  ratingGroup.add(FuzzyVariable("Excelent - Rating", 9.0f, 9.5f, 10.0f, 10.0f));

  genreGroup.add(FuzzyVariable("Very Low Interest - Genre", 0, 0, 1.5, 2));
  genreGroup.add(FuzzyVariable("Low Interest - Genre", 1, 2, 2.5, 4));
  genreGroup.add(FuzzyVariable("Medium Interest - Genre", 3.5, 4, 6, 7));
  genreGroup.add(FuzzyVariable("High Interest - Genre", 6.5, 7, 8, 9));
  genreGroup.add(FuzzyVariable("Very High Interest - Genre", 8.5, 9.5, 10, 10));

  releaseYearGroup.add(FuzzyVariable("Very Old - Release Year", 1900, 1900, 1950, 1960));
  releaseYearGroup.add(FuzzyVariable("Old - Release Year", 1950, 1960, 1970, 1980));
  releaseYearGroup.add(FuzzyVariable("Medium - Release Year", 1975, 1995, 2005, 2015));
  releaseYearGroup.add(FuzzyVariable("Recent - Release Year", 2010, 2015, 2020, 2025));
  releaseYearGroup.add(FuzzyVariable("Very Recent - Release Year", 2020, 2025, 2030, 2030));
}

void MovieRecommendation::readSpreadsheet()
{
  initializeGroups();
  std::unordered_map<std::string, float> genresPreference = {
    {"Crime", 10.f},
    {"Foreign", 9.5f},
    {"Horror", 9.f},
    {"Thriller", 9.f},
    {"Comedy", 8.5f},
    {"Drama", 8.5f},
    {"Fantasy", 8.f},
    {"Animation", 8.f},
    {"Mystery", 7.5f},
    {"Action", 7.f},
    {"Family", 6.5f},
    {"Adventure", 6.f},
    {"War", 5.5f},
    {"Romance", 5.f},
    {"History", 4.5f},
    {"Music", 4.f},
    {"Western", 3.5f},
    {"Documentary", 2.f},
    {"TV", .5f}
  };

  TablePrinter movieTable(
    { "Name", "Year", "Rating", "VotesQtd", "Genres" },
    { 23, 5, 9, 9, 23 }
  );
  movieTable.printHeader();

  try
  {
    typedef std::vector<std::vector<std::string>> Spreadsheet;
    const Spreadsheet &spreadsheet = Utils::readCSV("../../movie_dataset.csv");
    for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
    {
      const std::vector<std::string> &line = *it;
      if (line.size() <= 20) continue;

      float year, rating, qtdVotes, genreQuantitative = 0;

      try {
        std::string name = line[18].substr(0, 23);
        Utils::leftPadTo(name, 23);

        const std::vector<std::string> genres = Utils::split(line[2], ' ');

        if (genres.size() == 0) continue;

        int i = 0;
        for (std::string genre : genres) {
          i++;
          genreQuantitative += genresPreference[genre];
        }
        genreQuantitative /= i;

        rating = std::stof(line[19].c_str());
        qtdVotes = std::stof(line[20].c_str());

        const std::string releaseDate = line[12].substr(0, 4);
        year = std::stof(releaseDate);

        movieTable.printRow(name, year, rating, qtdVotes, line[2]);
      }
      catch (...) {
        continue;
      }

      std::unordered_map<std::string, float> variables;

      votesQuantityGroup.fuzzyfication(qtdVotes, variables);
      ratingGroup.fuzzyfication(rating, variables);
      genreGroup.fuzzyfication(genreQuantitative, variables);
      releaseYearGroup.fuzzyfication(year, variables);

      applyRules(variables);

      const float NA = variables["NA"];
      const float A = variables["A"];
      const float MA = variables["VA"];

      const float totalScore = NA + A + MA;
      const float score = totalScore != 0 ? (NA * 1.f + A * 7.f + MA * 10.f) / totalScore : 0;

      const Result result(line[18], score, NA, A, MA);
      results.push_back(result);
    }
  }
  catch (...)
  {
    std::cout << "\n\nErro ao ler arquivo\n\n";
  }
}

void MovieRecommendation::applyRules(
  std::unordered_map<std::string, float>& variables
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
  ruleAND(variables, "Very Low - Rating", "Votes_VeryLow", "NA");
  ruleAND(variables, "Very Low - Rating", "Votes_Low", "NA");
  ruleAND(variables, "Very Low - Rating", "Votes_Medium", "NA");
  ruleAND(variables, "Very Low - Rating", "Votes_High", "NA");
  ruleAND(variables, "Very Low - Rating", "Votes_VeryHigh", "NA");

  ruleAND(variables, "Low - Rating", "Votes_VeryLow", "NA");
  ruleAND(variables, "Low - Rating", "Votes_Low", "NA");
  ruleAND(variables, "Low - Rating", "Votes_Medium", "NA");
  ruleAND(variables, "Low - Rating", "Votes_High", "NA");
  ruleAND(variables, "Low - Rating", "Votes_VeryHigh", "NA");

  ruleAND(variables, "Medium - Rating", "Votes_VeryLow", "NA");
  ruleAND(variables, "Medium - Rating", "Votes_Low", "NA");
  ruleAND(variables, "Medium - Rating", "Votes_Medium", "A");
  ruleAND(variables, "Medium - Rating", "Votes_High", "A");
  ruleAND(variables, "Medium - Rating", "Votes_VeryHigh", "NA");

  ruleAND(variables, "Good - Rating", "Votes_VeryLow", "NA");
  ruleAND(variables, "Good - Rating", "Votes_Low", "NA");
  ruleAND(variables, "Good - Rating", "Votes_Medium", "NA");
  ruleAND(variables, "Good - Rating", "Votes_High", "A");
  ruleAND(variables, "Good - Rating", "Votes_VeryHigh", "A");

  ruleAND(variables, "Very Good - Rating", "Votes_VeryLow", "NA");
  ruleAND(variables, "Very Good - Rating", "Votes_Low", "NA");
  ruleAND(variables, "Very Good - Rating", "Votes_Medium", "A");
  ruleAND(variables, "Very Good - Rating", "Votes_High", "A");
  ruleAND(variables, "Very Good - Rating", "Votes_VeryHigh", "VA");

  ruleAND(variables, "Excelent - Rating", "Votes_VeryLow", "A");
  ruleAND(variables, "Excelent - Rating", "Votes_Low", "A");
  ruleAND(variables, "Excelent - Rating", "Votes_Medium", "A");
  ruleAND(variables, "Excelent - Rating", "Votes_High", "VA");
  ruleAND(variables, "Excelent - Rating", "Votes_VeryHigh", "VA");

  // Genre X Rating
  ruleAND(variables, "Very Low Interest - Genre", "NA", "NA");
  ruleAND(variables, "Very Low Interest - Genre", "A", "NA");
  ruleAND(variables, "Very Low Interest - Genre", "VA", "A");

  ruleAND(variables, "Low Interest - Genre", "NA", "NA");
  ruleAND(variables, "Low Interest - Genre", "A", "NA");
  ruleAND(variables, "Low Interest - Genre", "VA", "A");

  ruleAND(variables, "Medium Interest - Genre", "NA", "NA");
  ruleAND(variables, "Medium Interest - Genre", "A", "NA");
  ruleAND(variables, "Medium Interest - Genre", "VA", "A");

  ruleAND(variables, "High Interest - Genre", "NA", "NA");
  ruleAND(variables, "High Interest - Genre", "A", "A");
  ruleAND(variables, "High Interest - Genre", "VA", "VA");

  ruleAND(variables, "Very High Interest - Genre", "NA", "A");
  ruleAND(variables, "Very High Interest - Genre", "A", "VA");
  ruleAND(variables, "Very High Interest - Genre", "VA", "VA");
}
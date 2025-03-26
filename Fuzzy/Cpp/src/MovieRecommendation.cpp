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

  genreGroup.add(FuzzyVariable("Very Low Interest - 1st Genre", 0, 0, 1.5, 2));
  genreGroup.add(FuzzyVariable("Low Interest - 1st Genre", 1, 2, 2.5, 4));
  genreGroup.add(FuzzyVariable("Medium Interest - 1st Genre", 3.5, 4, 6, 7));
  genreGroup.add(FuzzyVariable("High Interest - 1st Genre", 6.5, 7, 8, 9));
  genreGroup.add(FuzzyVariable("Very High Interest - 1st Genre", 8.5, 9.5, 10, 10));
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
    { "Name", "Popularity", "Duration", "Rating", "VotesQtd", "Genres" },
    { 23, 11, 9, 9, 9, 23 }
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

      float popularity, runtime, rating, qtdVotes, genreQuantitative = 0;

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

        popularity = std::stof(line[9].substr(0, 6).c_str());
        runtime = std::stof(line[14].c_str());
        rating = std::stof(line[19].c_str());
        qtdVotes = std::stof(line[20].c_str());

        movieTable.printRow(name, popularity, runtime, rating, qtdVotes, line[2]);
      }
      catch (...) {
        continue;
      }

      std::unordered_map<std::string, float> variables;

      votesQuantityGroup.fuzzyfication(qtdVotes, variables);
      ratingGroup.fuzzyfication(rating, variables);
      genreGroup.fuzzyfication(genreQuantitative, variables);

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
  // Rating X QtdVotes
  ruleAND(variables, "Very Low - Rating", "Very Low - QtdVotes", "NA");
  ruleAND(variables, "Very Low - Rating", "Low - QtdVotes", "NA");
  ruleAND(variables, "Very Low - Rating", "Medium - QtdVotes", "NA");
  ruleAND(variables, "Very Low - Rating", "High - QtdVotes", "NA");
  ruleAND(variables, "Very Low - Rating", "Very High - QtdVotes", "NA");

  ruleAND(variables, "Low - Rating", "Very Low - QtdVotes", "NA");
  ruleAND(variables, "Low - Rating", "Low - QtdVotes", "NA");
  ruleAND(variables, "Low - Rating", "Medium - QtdVotes", "NA");
  ruleAND(variables, "Low - Rating", "High - QtdVotes", "NA");
  ruleAND(variables, "Low - Rating", "Very High - QtdVotes", "NA");

  ruleAND(variables, "Medium - Rating", "Very Low - QtdVotes", "NA");
  ruleAND(variables, "Medium - Rating", "Low - QtdVotes", "NA");
  ruleAND(variables, "Medium - Rating", "Medium - QtdVotes", "A");
  ruleAND(variables, "Medium - Rating", "High - QtdVotes", "A");
  ruleAND(variables, "Medium - Rating", "Very High - QtdVotes", "NA");

  ruleAND(variables, "Good - Rating", "Very Low - QtdVotes", "NA");
  ruleAND(variables, "Good - Rating", "Low - QtdVotes", "NA");
  ruleAND(variables, "Good - Rating", "Medium - QtdVotes", "NA");
  ruleAND(variables, "Good - Rating", "High - QtdVotes", "A");
  ruleAND(variables, "Good - Rating", "Very High - QtdVotes", "A");

  ruleAND(variables, "Very Good - Rating", "Very Low - QtdVotes", "NA");
  ruleAND(variables, "Very Good - Rating", "Low - QtdVotes", "NA");
  ruleAND(variables, "Very Good - Rating", "Medium - QtdVotes", "A");
  ruleAND(variables, "Very Good - Rating", "High - QtdVotes", "A");
  ruleAND(variables, "Very Good - Rating", "Very High - QtdVotes", "VA");

  ruleAND(variables, "Excelent - Rating", "Very Low - QtdVotes", "A");
  ruleAND(variables, "Excelent - Rating", "Low - QtdVotes", "A");
  ruleAND(variables, "Excelent - Rating", "Medium - QtdVotes", "A");
  ruleAND(variables, "Excelent - Rating", "High - QtdVotes", "VA");
  ruleAND(variables, "Excelent - Rating", "Very High - QtdVotes", "VA");

  // Genre X Rating
  ruleAND(variables, "Very Low Interest - 1st Genre", "NA", "NA");
  ruleAND(variables, "Very Low Interest - 1st Genre", "A", "NA");
  ruleAND(variables, "Very Low Interest - 1st Genre", "VA", "A");

  ruleAND(variables, "Low Interest - 1st Genre", "NA", "NA");
  ruleAND(variables, "Low Interest - 1st Genre", "A", "NA");
  ruleAND(variables, "Low Interest - 1st Genre", "VA", "A");

  ruleAND(variables, "Medium Interest - 1st Genre", "NA", "NA");
  ruleAND(variables, "Medium Interest - 1st Genre", "A", "NA");
  ruleAND(variables, "Medium Interest - 1st Genre", "VA", "A");

  ruleAND(variables, "High Interest - 1st Genre", "NA", "NA");
  ruleAND(variables, "High Interest - 1st Genre", "A", "A");
  ruleAND(variables, "High Interest - 1st Genre", "VA", "VA");

  ruleAND(variables, "Very High Interest - 1st Genre", "NA", "A");
  ruleAND(variables, "Very High Interest - 1st Genre", "A", "VA");
  ruleAND(variables, "Very High Interest - 1st Genre", "VA", "VA");
}
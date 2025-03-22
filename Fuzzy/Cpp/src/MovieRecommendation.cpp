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

void MovieRecommendation::fuzzyfication() const
{
  VariableGroup votesQuantityGroup;
  votesQuantityGroup.add(FuzzyVariable("Very Low - QtdVotes",  0,    0,    100,    200));
  votesQuantityGroup.add(FuzzyVariable("Low - QtdVotes",       150,  200,  250,    400));
  votesQuantityGroup.add(FuzzyVariable("Medium - QtdVotes",    300,  400,  500,    800));
  votesQuantityGroup.add(FuzzyVariable("High - QtdVotes",      700,  1000, 2000,   4000));
  votesQuantityGroup.add(FuzzyVariable("Very High - QtdVotes", 3500, 6000, 10000,  13000));

  VariableGroup ratingGroup;
  ratingGroup.add(FuzzyVariable("Very Bad - Rating",   0.0f, 0.0f, 1.0f, 3.0f));
  ratingGroup.add(FuzzyVariable("Bad - Rating",        2.0f, 3.0f, 4.0f, 5.0f));
  ratingGroup.add(FuzzyVariable("Medium - Rating",     4.5f, 5.0f, 5.5f, 6.0f));
  ratingGroup.add(FuzzyVariable("Good - Rating",       5.5f, 6.5f, 7.0f, 8.0f));
  ratingGroup.add(FuzzyVariable("Very Good - Rating",  7.5f, 8.2f, 9.0f, 9.5f));
  ratingGroup.add(FuzzyVariable("Excelent - Rating",   9.0f, 9.5f, 10.0f, 10.0f));

  VariableGroup runtimeGroup;
  runtimeGroup.add(FuzzyVariable("Very Short - Runtime",  0,    0,    20,   50));
  runtimeGroup.add(FuzzyVariable("Short - Runtime",       40,   55,   60,   75));
  runtimeGroup.add(FuzzyVariable("Medium - Runtime",      70,   80,   100,  120));
  runtimeGroup.add(FuzzyVariable("Long - Runtime",        100,  130,  150,  160));
  runtimeGroup.add(FuzzyVariable("Very Long - Runtime",   155,  170,  190,  190));

  std::unordered_map<std::string, float> genresPreference;
  
  genresPreference["Crime"] = 10;
  genresPreference["Foreign"] = 10;
  genresPreference["Horror"] = 10;
  genresPreference["Thriller"] = 10;
  genresPreference["Comedy"] = 9.5f;
  genresPreference["Drama"] = 9;
  genresPreference["Animation"] = 8.5f;
  genresPreference["Mystery"] = 8;
  genresPreference["Action"] = 7.5f;
  genresPreference["Family"] = 7;
  genresPreference["Adventure"] = 6;
  genresPreference["Fantasy"] = 6;
  genresPreference["War"] = 5;
  genresPreference["Romance"] = 4.5f;
  genresPreference["History"] = 4;
  genresPreference["Music"] = 4;
  genresPreference["Documentary"] = 3;
  genresPreference["Science"] = 2;
  genresPreference["Western"] = 1;
  genresPreference["TV"] = 0;

  VariableGroup firstGenreGroup;
  firstGenreGroup.add(FuzzyVariable("Very Low Interest - 1st Genre", 0, 0, 1.5, 2));
  firstGenreGroup.add(FuzzyVariable("Low Interest - 1st Genre", 1, 2, 2.5, 4));
  firstGenreGroup.add(FuzzyVariable("Medium Interest - 1st Genre", 3.5, 4, 6, 7));
  firstGenreGroup.add(FuzzyVariable("High Interest - 1st Genre", 6.5, 7, 8, 9));
  firstGenreGroup.add(FuzzyVariable("Very High Interest - 1st Genre", 8.5, 9.5, 10, 10));

  VariableGroup secondGenreGroup;
  secondGenreGroup.add(FuzzyVariable("Very Low Interest - 2st Genre", 0, 0, 1.5, 2));
  secondGenreGroup.add(FuzzyVariable("Low Interest - 2st Genre", 1, 2, 2.5, 4));
  secondGenreGroup.add(FuzzyVariable("Medium Interest - 2st Genre", 3.5, 4, 6, 7));
  secondGenreGroup.add(FuzzyVariable("High Interest - 2st Genre", 6.5, 7, 8, 9));
  secondGenreGroup.add(FuzzyVariable("Very High Interest - 2st Genre", 8.5, 9.5, 10, 10));

  std::vector<MovieResult> results;
  try
  {
    TablePrinter movieTable(
      { "Name", "Popularity", "Duration", "Rating", "VotesQtd", "Genres" }, 
      { 23, 11, 9, 9, 9, 23}
    );
    movieTable.printHeader();

    typedef std::vector<std::vector<std::string>> Spreadsheet;
    const Spreadsheet &spreadsheet = Utils::readCSV("../../movie_dataset.csv");
    for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
    {
      const std::vector<std::string> &line = *it;
      if (line.size() <= 20) continue;

      float popularity, runtime, rating, qtdVotes, firstGenrePreference, secondGenrePreference;
      bool hasSecondGenre = false;

      try {
        std::string name = line[7].substr(0, 23);
        Utils::leftPadTo(name, 23);

        const std::vector<std::string> genres = Utils::split(line[2], ' ');

        if (genres.size() == 0) continue;

        hasSecondGenre = genres.size() > 1;

        std::string firstGenre = genres[0], secondGenre = "";

        firstGenrePreference  = genresPreference[firstGenre];
        if (hasSecondGenre) {
          secondGenre = genres[1];
          secondGenrePreference = genresPreference[secondGenre];
        }
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
      runtimeGroup.fuzzyfication(runtime, variables);
      ratingGroup.fuzzyfication(rating, variables);
      firstGenreGroup.fuzzyfication(firstGenrePreference, variables);
      if (hasSecondGenre)
        secondGenreGroup.fuzzyfication(secondGenrePreference, variables);

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

      // 1st Genre X 2nd Genre
      if (hasSecondGenre) {
        ruleAND(variables, "Very Low Interest - 1st Genre", "Very Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Very Low Interest - 1st Genre", "Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Very Low Interest - 1st Genre", "Medium Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Very Low Interest - 1st Genre", "High Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "Very Low Interest - 1st Genre", "Very High Interest - 2st Genre", "Genre_A");

        ruleAND(variables, "Low Interest - 1st Genre", "Very Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Low Interest - 1st Genre", "Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Low Interest - 1st Genre", "Medium Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Low Interest - 1st Genre", "High Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "Low Interest - 1st Genre", "Very High Interest - 2st Genre", "Genre_VA");

        ruleAND(variables, "Medium Interest - 1st Genre", "Very Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Medium Interest - 1st Genre", "Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Medium Interest - 1st Genre", "Medium Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "Medium Interest - 1st Genre", "High Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "Medium Interest - 1st Genre", "Very High Interest - 2st Genre", "Genre_VA");

        ruleAND(variables, "High Interest - 1st Genre", "Very Low Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "High Interest - 1st Genre", "Low Interest - 2st Genre", "Genre_A");
        ruleAND(variables, "High Interest - 1st Genre", "Medium Interest - 2st Genre", "Genre_A");

        ruleAND(variables, "Very High Interest - 1st Genre", "Very Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Very High Interest - 1st Genre", "Low Interest - 2st Genre", "Genre_NA");
        ruleAND(variables, "Very High Interest - 1st Genre", "Medium Interest - 2st Genre", "Genre_NA");

        ruleOR(variables, "High Interest - 1st Genre", "High Interest - 2st Genre", "Genre_VA");
        ruleOR(variables, "High Interest - 1st Genre", "Very High Interest - 2st Genre", "Genre_VA");
        ruleOR(variables, "Very High Interest - 1st Genre", "High Interest - 2st Genre", "Genre_VA");
        ruleOR(variables, "Very High Interest - 1st Genre", "Very High Interest - 2st Genre", "Genre_VA");

        // Gender Atractiveness X Atractiveness
        ruleAND(variables, "Gender_NA", "NA", "NA");
        ruleAND(variables, "Gender_NA", "A", "NA");
        ruleAND(variables, "Gender_NA", "VA", "A");

        ruleAND(variables, "Gender_A", "NA", "NA");
        ruleAND(variables, "Gender_A", "A", "A");
        ruleAND(variables, "Gender_A", "VA", "VA");

        ruleAND(variables, "Gender_VA", "NA", "A");
        ruleAND(variables, "Gender_VA", "A", "VA");
        ruleAND(variables, "Gender_VA", "VA", "VA");
      }
      else {
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

      // Atractiveness X Runtime
      ruleAND(variables, "NA", "Very Short - Runtime", "NA");
      ruleAND(variables, "NA", "Short - Runtime", "NA");
      ruleAND(variables, "NA", "Medium - Runtime", "NA");
      ruleAND(variables, "NA", "Long - Runtime", "NA");
      ruleAND(variables, "NA", "Very Long - Runtime", "NA");

      ruleAND(variables, "A", "Very Short - Runtime", "VA");
      ruleAND(variables, "A", "Short - Runtime", "NA");
      ruleAND(variables, "A", "Medium - Runtime", "A");
      ruleAND(variables, "A", "Long - Runtime", "A");
      ruleAND(variables, "A", "Very Long - Runtime", "NA");

      ruleAND(variables, "VA", "Very Short - Runtime", "VA");
      ruleAND(variables, "VA", "Short - Runtime", "VA");
      ruleAND(variables, "VA", "Medium - Runtime", "VA");
      ruleAND(variables, "VA", "Long - Runtime", "VA");
      ruleAND(variables, "VA", "Very Long - Runtime", "NA");


      const float NA = variables["NA"];
      const float A = variables["A"];
      const float MA = variables["VA"];

      const float score = (NA * 1.5f + A * 7.0f + MA * 9.5f) / (NA + A + MA);

      MovieResult result;
      result.name = line[7];
      result.score = score;
      result.nonAtractiveScore = NA;
      result.atractiveScore = A;
      result.veryAtractiveScore = MA;

      results.push_back(result);
    }
  }
  catch (...)
  {
    std::cout << "\n\nErro ao ler arquivo\n\n";
  }

  std::sort(results.begin(), results.end(), [](const MovieResult& mr1, const MovieResult& mr2) { return mr2.score < mr1.score; });

  std::cout << "\n\n";

  TablePrinter table({ "Name", "NA", "A", "VA", "Score" }, { 32, 9, 9, 9, 9 });
  table.printHeader();

  for (size_t i = 0; i != 10; i++) {
    const MovieResult& result = results[i];
    table.printRow(result.name, result.nonAtractiveScore, result.atractiveScore, result.veryAtractiveScore, result.score);
  }
}
#include "MovieRecommendation.h"

#include "Utils.h"
#include "VariableGroup.h"

#include <iostream>

void MovieRecommendation::fuzzyfication() const
{
  VariableGroup votesQuantityGroup;
  votesQuantityGroup.add(FuzzyVariable("Very Low",  0,    0,    100,    200));
  votesQuantityGroup.add(FuzzyVariable("Low",       150,  200,  250,    400));
  votesQuantityGroup.add(FuzzyVariable("Medium",    300,  400,  500,    800));
  votesQuantityGroup.add(FuzzyVariable("High",      700,  1000, 2000,   4000));
  votesQuantityGroup.add(FuzzyVariable("Very High", 3500, 6000, 10000,  13000));

  VariableGroup ratingGroup;
  ratingGroup.add(FuzzyVariable("Very Bad",   0.0f, 0.0f, 1.0f, 3.0f));
  ratingGroup.add(FuzzyVariable("Bad",        2.0f, 3.0f, 4.0f, 5.0f));
  ratingGroup.add(FuzzyVariable("Medium",     4.5f, 5.0f, 5.5f, 6.0f));
  ratingGroup.add(FuzzyVariable("Good",       5.5f, 6.5f, 7.0f, 8.0f));
  ratingGroup.add(FuzzyVariable("Very Good",  7.5f, 8.2f, 9.0f, 9.5f));
  ratingGroup.add(FuzzyVariable("Excelent",   9.0f, 9.5f, 10.0f, 10.0f));

  VariableGroup runtimeGroup;
  runtimeGroup.add(FuzzyVariable("Very Short",  0,    0,    20,   50));
  runtimeGroup.add(FuzzyVariable("Short",       40,   55,   60,   75));
  runtimeGroup.add(FuzzyVariable("Medium",      70,   80,   100,  120));
  runtimeGroup.add(FuzzyVariable("Long",        100,  130,  150,  160));
  runtimeGroup.add(FuzzyVariable("Very Long",   155,  170,  190,  190));

  try
  {
    std::cout << "Nome\t\t\tPopularidade\tDuração\tAvaliação\tQtdVotos" << std::endl;

    typedef std::vector<std::vector<std::string>> Spreadsheet;
    const Spreadsheet &spreadsheet = Utils::readCSV("../../movie_dataset.csv");
    for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
    {
      const std::vector<std::string> &line = *it;
      if (line.size() <= 20)
        continue;

      std::unordered_map<std::string, float> variables;

      try {
        std::string name = line[7].substr(0, 23);
        Utils::leftPadTo(name, 23);

        const float popularity = std::stof(line[9].substr(0, 6).c_str());
        const float runtime = std::stof(line[14].c_str());
        const float rating = std::stof(line[19].c_str());
        const float qtdVotes = std::stof(line[20].c_str());

        std::cout << name << "\t" << popularity << "\t\t" << runtime << "\t\t" << rating << "\t\t" << qtdVotes << std::endl;
      }
      catch (...) {
        continue;
      }

      // TODO: Implementar a leitura das variáveis do arquivo CSV

      const float NA = variables["NA"];
      const float A = variables["A"];
      const float MA = variables["MA"];

      const float score = (NA * 1.5f + A * 7.0f + MA * 9.5f) / (NA + A + MA);
    }
  }
  catch (...)
  {
    std::cout << "\n\nErro ao ler arquivo\n\n";
  }
}
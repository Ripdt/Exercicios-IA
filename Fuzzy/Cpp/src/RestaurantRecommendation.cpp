#include "RestaurantRecommendation.h"

#include "VariableGroup.h"
#include "Utils.h"

#include <iostream>

void RestaurantRecommendation::fuzzyfication() const
{
  VariableGroup costGroup;
  costGroup.add(FuzzyVariable("Very Cheap", 0, 0, 10, 20));
  costGroup.add(FuzzyVariable("Cheap", 10, 20, 30, 60));
  costGroup.add(FuzzyVariable("Medium", 20, 40, 50, 70));
  costGroup.add(FuzzyVariable("Expensive", 40, 60, 70, 120));
  costGroup.add(FuzzyVariable("Very Expensive", 70, 110, 500, 500));

  VariableGroup grupoRating;
  grupoRating.add(FuzzyVariable("VB", 0, 0, 10, 20));
  grupoRating.add(FuzzyVariable("B", 10, 20, 30, 40));
  grupoRating.add(FuzzyVariable("G", 20, 40, 45, 50));
  grupoRating.add(FuzzyVariable("VG", 40, 48, 50, 50));

  VariableGroup grupoVotos;
  grupoVotos.add(FuzzyVariable("V_VL", 0, 0, 10, 20));
  grupoVotos.add(FuzzyVariable("V_L", 10, 20, 50, 60));
  grupoVotos.add(FuzzyVariable("V_M", 40, 80, 200, 300));
  grupoVotos.add(FuzzyVariable("V_H", 200, 300, 500, 1000));
  grupoVotos.add(FuzzyVariable("V_VH", 400, 500, 3200, 3200));

  VariableGroup grupoAtratividade;
  grupoRating.add(FuzzyVariable("NA", 0, 0, 3, 6));
  grupoRating.add(FuzzyVariable("A", 5, 7, 8, 10));
  grupoRating.add(FuzzyVariable("VA", 7, 9, 10, 10));

  try
  {
    std::cout << "Nome\t\t\tCozinha\t\t\tCusto\tRating\tVotos\tNA\tA\tMA\tScore" << std::endl;
    typedef std::vector<std::vector<std::string>> Spreadsheet;
    const Spreadsheet &spreadsheet = Utils::readCSV("../../restaurantes_filtrados.csv");
    for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
    {
      const std::vector<std::string> &line = *it;
      std::unordered_map<std::string, float> variables;

      std::string name = line[1].substr(0, 23);
      Utils::leftPadTo(name, 23);

      std::string cuisine = line[2].substr(0, 23);
      Utils::leftPadTo(cuisine, 23);

      const float cost = std::stof(line[3].substr(0,6).c_str());
      costGroup.fuzzyfication(cost, variables);

      const float rating = std::stof(line[5].substr(0, 6).c_str());
      grupoRating.fuzzyfication(rating, variables);

      const float votes = std::stof(line[8].substr(0, 6).c_str());
      grupoVotos.fuzzyfication(votes, variables);

      std::cout << name << "\t" << cuisine << "\t" << cost << "\t" << rating << "\t" << votes << "\t";

      ruleAND(variables, "Cheap", "G", "A");
      ruleAND(variables, "Very Cheap", "G", "A");
      ruleAND(variables, "Very Cheap", "VG", "VA");
      ruleAND(variables, "Cheap", "VG", "VA");
      ruleAND(variables, "Cheap", "B", "NA");
      ruleAND(variables, "Very Cheap", "B", "A");
      ruleAND(variables, "Very Cheap", "VB", "NA");
      ruleAND(variables, "Very Expensive", "VB", "NA");
      ruleAND(variables, "Very Expensive", "B", "NA");
      ruleAND(variables, "Very Expensive", "G", "NA");
      ruleAND(variables, "Very Expensive", "VG", "A");

      ruleAND(variables, "VA", "V_VL", "NA");
      ruleAND(variables, "VA", "V_L", "A");
      ruleAND(variables, "VA", "V_M", "A");

      ruleAND(variables, "A", "V_VL", "NA");
      ruleAND(variables, "A", "V_L", "NA");
      ruleAND(variables, "A", "V_M", "NA");

      const float NA = variables["NA"];
      const float A = variables["A"];
      const float VA = variables["VA"];

      const float score = (NA * 1.5f + A * 7.0f + VA * 9.5f) / (NA + A + VA);

      std::cout << std::to_string(NA).substr(0,6) << "\t" << std::to_string(A).substr(0, 6) << "\t" << std::to_string(VA).substr(0, 6) << "\t" << score << std::endl;
    }
  }
  catch (...)
  {
    std::cout << "\n\nErro ao ler arquivo\n\n";
  }
}
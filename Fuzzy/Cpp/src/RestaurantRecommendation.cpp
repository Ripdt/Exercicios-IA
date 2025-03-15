#include "RestaurantRecommendation.h"

#include "VariableGroup.h"
#include "Utils.h"

#include <iostream>

void RestaurantRecommendation::fuzzyfication() const
{
    VariableGroup grupoPreco;
    grupoPreco.add(FuzzyVariable("Muito Barato", 0, 0, 10, 20));
    grupoPreco.add(FuzzyVariable("Barato", 10, 20, 30, 60));
    grupoPreco.add(FuzzyVariable("Custo Medio", 20, 40, 50, 70));
    grupoPreco.add(FuzzyVariable("Caro", 40, 60, 70, 120));
    grupoPreco.add(FuzzyVariable("Muito Caro", 70, 110, 500, 500));

    VariableGroup grupoRating;
    grupoRating.add(FuzzyVariable("MR", 0, 0, 10, 20));
    grupoRating.add(FuzzyVariable("R", 10, 20, 30, 40));
    grupoRating.add(FuzzyVariable("B", 20, 40, 45, 50));
    grupoRating.add(FuzzyVariable("MB", 40, 48, 50, 50));

    VariableGroup grupoVotos;
    grupoVotos.add(FuzzyVariable("V_MPV", 0, 0, 10, 20));
    grupoVotos.add(FuzzyVariable("V_PV", 10, 20, 50, 60));
    grupoVotos.add(FuzzyVariable("V_MEV", 40, 80, 200, 300));
    grupoVotos.add(FuzzyVariable("V_BAV", 200, 300, 500, 1000));
    grupoVotos.add(FuzzyVariable("V_MUV", 400, 500, 3200, 3200));

    VariableGroup grupoAtratividade;
    grupoRating.add(FuzzyVariable("NA", 0, 0, 3, 6));
    grupoRating.add(FuzzyVariable("A", 5, 7, 8, 10));
    grupoRating.add(FuzzyVariable("MA", 7, 9, 10, 10));

    try
    {
      typedef std::vector<std::vector<std::string>> Spreadsheet;
      const Spreadsheet& spreadsheet = Utils::readCSV("../../restaurantes_filtrados.csv");
      for (Spreadsheet::const_iterator it = spreadsheet.begin() + 1; it != spreadsheet.end(); ++it)
      {
        const std::vector<std::string>& line = *it;
        std::unordered_map<std::string, float> variables;

        const float cost = std::stof(line[3].c_str());
        grupoPreco.fuzzyfication(cost, variables);

        const float rating = std::stof(line[5].c_str());
        grupoRating.fuzzyfication(rating, variables);

        const float votos = std::stof(line[8].c_str());
        grupoVotos.fuzzyfication(votos, variables);

        std::cout << line[2].c_str() << " - custodinheiro " << cost << " rating " << rating << " votos " << votos << std::endl;

        // Barato e B -> A
        // Muito Barato e B -> A
        // Muito Barato e MB -> MA
        // Barato e MB -> MA
        // Barato e R -> NA
        // Muito Barato e R -> A
        // Muito Barato e MR -> NA

        ruleAND(variables, "Barato", "B", "A");
        ruleAND(variables, "Muito Barato", "B", "A");
        ruleAND(variables, "Muito Barato", "MB", "MA");
        ruleAND(variables, "Barato", "MB", "MA");
        ruleAND(variables, "Barato", "R", "NA");
        ruleAND(variables, "Muito Barato", "R", "A");
        ruleAND(variables, "Muito Barato", "MR", "NA");
        ruleAND(variables, "Muito Caro", "MR", "NA");
        ruleAND(variables, "Muito Caro", "R", "NA");
        ruleAND(variables, "Muito Caro", "B", "NA");
        ruleAND(variables, "Muito Caro", "MB", "A");

        ruleAND(variables, "MA", "V_MPV", "NA");
        ruleAND(variables, "MA", "V_PV", "A");
        ruleAND(variables, "MA", "V_MEV", "A");

        ruleAND(variables, "A", "V_MPV", "NA");
        ruleAND(variables, "A", "V_PV", "NA");
        ruleAND(variables, "A", "V_MEV", "NA");

        const float NA = variables["NA"];
        const float A = variables["A"];
        const float MA = variables["MA"];

        const float score = (NA * 1.5f + A * 7.0f + MA * 9.5f) / (NA + A + MA);

        std::cout << "NA " << NA << " A " << A << " MA " << MA << std::endl;
        std::cout << "\t" << cost << " " << rating << "-> " << score << std::endl;
      }
    }
    catch (...)
    {
        std::cout << "\n\nErro ao ler arquivo\n\n";
    }
}
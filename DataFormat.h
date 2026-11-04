#pragma once
#include <string>
#include <vector>
#include "Graph.h" 

class DataFormat {
public:
    static void loadIMDbData(const std::string& filename, Graph& graph);

private:
    static std::vector<std::string> parseCSVLine(const std::string& line);
    static std::vector<std::string> splitActors(const std::string& actorsStr);
    static std::string trim(const std::string& str);
};
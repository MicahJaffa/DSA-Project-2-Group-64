#pragma once

#include "graph.h"

#include <string>

class DataFormat {
public:
    static Graph loadIMDbData(const std::string& filename);
    static std::string trim(const std::string& text);
};

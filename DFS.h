#pragma once
#include <string>
#include <vector>
#include "Graph.h"

std::vector<std::string> DFS(Graph& graph,
    const std::string& startActor,
    const std::string& targetActor);

void printDFSPath(const std::vector<std::string>& path);

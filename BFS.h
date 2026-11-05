#pragma once
#include <string>
#include <vector>
#include "Graph.h"

std::vector<std::string> BFS(Graph& graph,
    const std::string& startActor,
    const std::string& targetActor);
void printBFSPath(const std::vector<std::string>& path);
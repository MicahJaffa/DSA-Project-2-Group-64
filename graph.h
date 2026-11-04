#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> actorToMovies;
    std::unordered_map<std::string, std::vector<std::string>> movieToActors;

public:
    Graph();
    void printTest(int limit = 5);
    void insertEdge(const std::string& actor, const std::string& movie);
    std::vector<std::string> getMoviesForActor(const std::string& actor);
    std::vector<std::string> getActorsInMovie(const std::string& movie);
};
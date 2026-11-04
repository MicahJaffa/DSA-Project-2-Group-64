#include "graph.h"
#include <iostream>
Graph::Graph() {}
void Graph::insertEdge(const std::string& actor, const std::string& movie)
{
    actorToMovies[actor].push_back(movie);
    movieToActors[movie].push_back(actor);
}

std::vector<std::string> Graph::getMoviesForActor(const std::string& actor)
{
    if (actorToMovies.find(actor) != actorToMovies.end()) {
        return actorToMovies[actor];
    }
    return {};
}

std::vector<std::string> Graph::getActorsInMovie(const std::string& movie)
{
    if (movieToActors.find(movie) != movieToActors.end()) {
        return movieToActors[movie];
    }
    return {};
}
void Graph::printTest(int limit) {
    int count = 0;
    std::cout << "--- Graph Sample ---" << std::endl;

    for (const auto& pair : actorToMovies) {
        if (count >= limit) break;

        std::cout << "Actor: " << pair.first << "\nMovies: ";

        for (size_t i = 0; i < pair.second.size(); ++i) {
            std::cout << pair.second[i];
            if (i < pair.second.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n\n";
        count++;
    }
    std::cout << "--------------------" << std::endl;
}
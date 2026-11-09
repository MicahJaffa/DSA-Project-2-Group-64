#include "DataFormat.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

bool hasActor(const std::vector<std::string>& cast, const std::string& actor) {
    return std::find(cast.begin(), cast.end(), actor) != cast.end();
}

void checkPath(const Graph& graph, const ConnectionResult& result, const std::string& start,
               const std::string& target, std::size_t distance) {
    require(result.status == ConnectionStatus::found, "Expected a connection");
    require(result.steps.size() == distance, "Incorrect connection count");
    std::string current = start;
    for (const auto& step : result.steps) {
        require(step.fromActor == current, "Gap between adjacent steps");
        const auto cast = graph.getActorsInMovie(step.movie.imdbId);
        require(hasActor(cast, current) && hasActor(cast, step.toActor), "Movie is missing a co-star");
        current = step.toActor;
    }
    require(current == target, "Path ends at the wrong actor");
}

void checkExample(Graph& graph) {
    require(graph.movieCount() == 4 && graph.actorCount() == 5, "Incorrect graph size");
    require(graph.getActorsInMovie("tt1") == std::vector<std::string>{"A", "B"}, "Duplicate or untrimmed credit");
    require(graph.getMoviesForActor("B").size() == 2, "Incorrect movie list for B");
    const auto chain = graph.shortestConnection("A", "C");
    checkPath(graph, chain, "A", "C", 2);
    require(chain.steps[0].movie.imdbId == "tt1" && chain.steps[0].toActor == "B", "Incorrect first step");
    require(chain.steps[1].movie.title == "Movie \"Two\", Part 2" && chain.steps[1].movie.year == "1991",
            "Incorrect movie details in second step");
    checkPath(graph, graph.shortestConnection("C", "A"), "C", "A", 2);
    checkPath(graph, graph.shortestConnection("A", "A"), "A", "A", 0);

    graph.addMovie({"tt5", "Shortcut", "2002"}, {"C", "A"});
    const auto direct = graph.shortestConnection("A", "C");
    checkPath(graph, direct, "A", "C", 1);
    require(direct.steps.front().movie.imdbId == "tt5", "Missed the shortcut");
    require(graph.shortestConnection("B", "D").status == ConnectionStatus::disconnected, "Remake joined two casts");
    require(graph.shortestConnection("X", "A").status == ConnectionStatus::unknownStart, "Unknown start");
    require(graph.shortestConnection("A", "X").status == ConnectionStatus::unknownTarget, "Unknown target");
    require(graph.shortestConnection("X", "X").status == ConnectionStatus::unknownBoth, "Unknown same-name query");
}

void checkIMDb(const Graph& graph) {
    const auto original = graph.getActorsInMovie("tt0047437");
    const auto remake = graph.getActorsInMovie("tt0114319");
    require(hasActor(original, "Audrey Hepburn") && !hasActor(original, "Harrison Ford"), "Incorrect 1954 Sabrina cast");
    require(hasActor(remake, "Harrison Ford") && !hasActor(remake, "Audrey Hepburn"), "Incorrect 1995 Sabrina cast");
    checkPath(graph, graph.shortestConnection("Audrey Hepburn", "Harrison Ford"), "Audrey Hepburn", "Harrison Ford", 2);
    checkPath(graph, graph.shortestConnection("Tom Hanks", "Tim Allen"), "Tom Hanks", "Tim Allen", 1);
}
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 2;
    }
    try {
        auto graph = DataFormat::loadIMDbData(argv[2]);
        const std::string suite = argv[1];
        if (suite == "example") {
            checkExample(graph);
        } else if (suite == "imdb") {
            checkIMDb(graph);
        } else {
            return 2;
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}

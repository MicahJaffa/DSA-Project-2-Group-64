#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

struct Movie {
    std::string imdbId;
    std::string title;
    std::string year;
};

struct ConnectionStep {
    std::string fromActor;
    Movie movie;
    std::string toActor;
};

enum class ConnectionStatus {
    found,
    unknownStart,
    unknownTarget,
    unknownBoth,
    disconnected
};

struct ConnectionResult {
    ConnectionStatus status;
    std::vector<ConnectionStep> steps;
};

class Graph {
public:
    void addMovie(const Movie& movie, const std::vector<std::string>& cast);
    std::vector<Movie> getMoviesForActor(const std::string& name) const;
    std::vector<std::string> getActorsInMovie(const std::string& imdbId) const;
    ConnectionResult shortestConnection(const std::string& start, const std::string& target) const;
    std::size_t actorCount() const;
    std::size_t movieCount() const;

private:
    struct Actor {
        std::string name;
        std::vector<std::size_t> movies;
    };

    struct Film {
        Movie details;
        std::vector<std::size_t> cast;
    };

    std::vector<Actor> actors;
    std::vector<Film> movies;
    std::unordered_map<std::string, std::size_t> actorByName;
    std::unordered_map<std::string, std::size_t> movieById;
};

#include "graph.h"

#include <algorithm>
#include <queue>
#include <utility>

void Graph::addMovie(const Movie& movie, const std::vector<std::string>& cast) {
    const auto filmEntry = movieById.emplace(movie.imdbId, movies.size());
    if (filmEntry.second) {
        movies.push_back({movie, {}});
    }
    const auto movieIndex = filmEntry.first->second;
    auto& members = movies[movieIndex].cast;
    for (const auto& name : cast) {
        if (name.empty()) {
            continue;
        }
        const auto actorEntry = actorByName.emplace(name, actors.size());
        if (actorEntry.second) {
            actors.push_back({name, {}});
        }
        const auto actorIndex = actorEntry.first->second;
        if (std::find(members.begin(), members.end(), actorIndex) == members.end()) {
            members.push_back(actorIndex);
            actors[actorIndex].movies.push_back(movieIndex);
        }
    }
}

std::vector<Movie> Graph::getMoviesForActor(const std::string& name) const {
    std::vector<Movie> result;
    const auto actor = actorByName.find(name);
    if (actor != actorByName.end()) {
        for (auto movie : actors[actor->second].movies) {
            result.push_back(movies[movie].details);
        }
    }
    return result;
}

std::vector<std::string> Graph::getActorsInMovie(const std::string& imdbId) const {
    std::vector<std::string> result;
    const auto film = movieById.find(imdbId);
    if (film != movieById.end()) {
        for (auto actor : movies[film->second].cast) {
            result.push_back(actors[actor].name);
        }
    }
    return result;
}

ConnectionResult Graph::shortestConnection(const std::string& start, const std::string& target) const {
    const auto startEntry = actorByName.find(start);
    const auto targetEntry = actorByName.find(target);
    if (startEntry == actorByName.end()) {
        const auto status = targetEntry == actorByName.end()
            ? ConnectionStatus::unknownBoth : ConnectionStatus::unknownStart;
        return {status, {}};
    }
    if (targetEntry == actorByName.end()) {
        return {ConnectionStatus::unknownTarget, {}};
    }
    const auto source = startEntry->second;
    const auto destination = targetEntry->second;
    if (source == destination) {
        return {ConnectionStatus::found, {}};
    }

    struct Predecessor {
        std::size_t actor;
        std::size_t movie;
    };
    const auto unseen = actors.size();
    std::vector<Predecessor> previous(actors.size(), {unseen, movies.size()});
    std::vector<bool> expandedMovie(movies.size(), false);
    std::queue<std::size_t> pending;
    previous[source].actor = source;
    pending.push(source);

    while (!pending.empty() && previous[destination].actor == unseen) {
        const auto current = pending.front();
        pending.pop();
        for (auto movie : actors[current].movies) {
            if (expandedMovie[movie]) {
                continue;
            }
            expandedMovie[movie] = true;
            for (auto costar : movies[movie].cast) {
                if (previous[costar].actor == unseen) {
                    previous[costar] = {current, movie};
                    pending.push(costar);
                }
            }
        }
    }
    if (previous[destination].actor == unseen) {
        return {ConnectionStatus::disconnected, {}};
    }

    std::vector<ConnectionStep> steps;
    for (auto actor = destination; actor != source; actor = previous[actor].actor) {
        const auto& link = previous[actor];
        steps.push_back({actors[link.actor].name, movies[link.movie].details, actors[actor].name});
    }
    std::reverse(steps.begin(), steps.end());
    return {ConnectionStatus::found, std::move(steps)};
}

std::size_t Graph::actorCount() const {
    return actors.size();
}

std::size_t Graph::movieCount() const {
    return movies.size();
}

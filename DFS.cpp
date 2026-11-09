#include "DFS.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

vector<string> DFS(Graph& graph, const string& startActor, const string& targetActor) {
    if (graph.getMoviesForActor(startActor).empty() ||
        graph.getMoviesForActor(targetActor).empty()) {
        return {};
    }

    if (startActor == targetActor) {
        return {startActor};
    }

    stack<string> actorsToVisit;
    unordered_set<string> visitedActors;
    unordered_set<string> visitedMovies;
    unordered_map<string, pair<string, Movie>> previous;

    actorsToVisit.push(startActor);
    visitedActors.insert(startActor);

    while (!actorsToVisit.empty()) {
        string currentActor = actorsToVisit.top();
        actorsToVisit.pop();

        for (const Movie& movie : graph.getMoviesForActor(currentActor)) {
            if (!visitedMovies.insert(movie.imdbId).second) {
                continue;
            }

            for (const string& coStar : graph.getActorsInMovie(movie.imdbId)) {
                if (!visitedActors.insert(coStar).second) {
                    continue;
                }

                previous[coStar] = {currentActor, movie};

                if (coStar == targetActor) {
                    vector<string> path;
                    string actor = targetActor;

                    while (actor != startActor) {
                        path.push_back(actor);
                        path.push_back(previous.at(actor).second.title);
                        actor = previous.at(actor).first;
                    }

                    path.push_back(startActor);
                    reverse(path.begin(), path.end());
                    return path;
                }

                actorsToVisit.push(coStar);
            }
        }
    }

    return {};
}

void printDFSPath(const vector<string>& path) {
    if (path.empty()) {
        cout << "No connection found (or an actor is missing from the data)." << endl;
        return;
    }

    cout << "DFS connection (" << (path.size() - 1) / 2
         << " movie connections; not necessarily shortest):" << endl;

    for (const string& name : path) {
        cout << name << endl;
    }
}

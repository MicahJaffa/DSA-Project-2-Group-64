#include "BFS.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<string> BFS(Graph& graph, const string& startActor, const string& targetActor) {
    if (graph.getMoviesForActor(startActor).empty() ||
        graph.getMoviesForActor(targetActor).empty()) {
        return {};
    }
    if (startActor == targetActor) {
        return {startActor};
    }

    queue<string> actorsToVisit;
    unordered_set<string> visitedActors;
    unordered_set<string> visitedMovies;
    unordered_map<string, pair<string, string>> previous;

    actorsToVisit.push(startActor);
    visitedActors.insert(startActor);

    while (!actorsToVisit.empty()) {
        string currentActor = actorsToVisit.front();
        actorsToVisit.pop();
        for (const string& movie : graph.getMoviesForActor(currentActor)) {
            if (!visitedMovies.insert(movie).second) {
                continue;
            }
            for (const string& coStar : graph.getActorsInMovie(movie)) {
                if (!visitedActors.insert(coStar).second) {
                    continue;
                }
                previous[coStar] = {currentActor, movie};
                if (coStar == targetActor) {
                    vector<string> path;
                    string actor = targetActor;
                    while (actor != startActor) {
                        path.push_back(actor);
                        path.push_back(previous.at(actor).second);
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

void printBFSPath(const vector<string>& path) {
    if (path.empty()) {
        cout << "No connection found (or an actor is missing from the data)." << endl;
        return;
    }
    cout << "Shortest connection (" << (path.size() - 1) / 2
         << " movie connections):" << endl;
    for (const string& name : path) {
        cout << name << endl;
    }
}
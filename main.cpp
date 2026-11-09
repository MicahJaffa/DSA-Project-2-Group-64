#include <iostream>
#include <string>
#include <chrono>
#include "BFS.h"
#include "DFS.h"
#include "DataFormat.h"
#include "Graph.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    string filename = "IMDb movies.csv";
    if (argc > 1) {
        filename = argv[1];
    }

    cout << "Loading graph from " << filename << "..." << endl;
    Graph graph;
    try {
        graph = DataFormat::loadIMDbData(filename);
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    cout << "Graph loaded with " << graph.actorCount() << " actors and "
        << graph.movieCount() << " movies." << endl;

    string startActor, targetActor;
    cout << "\nEnter start actor: ";
    getline(cin, startActor);
    cout << "Enter target actor: ";
    getline(cin, targetActor);

    // BFS Timing
    cout << "\n--- BFS Search ---" << endl;
    auto startBFS = high_resolution_clock::now();
    vector<string> pathBFS = BFS(graph, startActor, targetActor);
    auto endBFS = high_resolution_clock::now();

    auto durationBFS = duration_cast<milliseconds>(endBFS - startBFS);
    printBFSPath(pathBFS);
    cout << "BFS Time: " << durationBFS.count() << " ms\n" << endl;

    // DFS Timing
    cout << "--- DFS Search ---" << endl;
    auto startDFS = high_resolution_clock::now();
    vector<string> pathDFS = DFS(graph, startActor, targetActor);
    auto endDFS = high_resolution_clock::now();

    auto durationDFS = duration_cast<milliseconds>(endDFS - startDFS);
    printDFSPath(pathDFS);
    cout << "DFS Time: " << durationDFS.count() << " ms\n" << endl;

    return 0;
}
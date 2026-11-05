#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Graph.h"
#include "DataFormat.h"
#include "BFS.h"

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Graph movieGraph;

    cout << "Loading IMDb data..." << endl;
    DataFormat::loadIMDbData("IMDb movies.csv", movieGraph);

    string startActor, targetActor;

    cout << "Enter first actor: ";
    getline(cin, startActor);

    cout << "Enter second actor: ";
    getline(cin, targetActor);

    printBFSPath(BFS(movieGraph, startActor, targetActor));

    return 0;
}
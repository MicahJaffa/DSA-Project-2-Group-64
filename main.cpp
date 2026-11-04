#include <iostream>
#include <windows.h>
#include "Graph.h"
#include "DataFormat.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Graph movieGraph;

    cout << "Loading IMDb data... (This may take a moment)" << endl;


    DataFormat::loadIMDbData("IMDb movies.csv", movieGraph);

    cout << "Data loaded successfully!" << endl;

    movieGraph.printTest(10);

    return 0;
}
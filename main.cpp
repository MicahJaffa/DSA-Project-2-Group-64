#include "DataFormat.h"

#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
bool readActor(const char* prompt, std::string& name) {
    for (;;) {
        std::cout << prompt;
        if (!std::getline(std::cin, name)) {
            return false;
        }
        name = DataFormat::trim(name);
        if (name == "quit") {
            return false;
        }
        if (!name.empty()) {
            return true;
        }
        std::cout << "Enter a full actor name.\n";
    }
}

void printConnection(const ConnectionResult& result, const std::string& start, const std::string& target) {
    switch (result.status) {
        case ConnectionStatus::unknownStart:
            std::cout << "Unknown actor: " << start << '\n';
            return;
        case ConnectionStatus::unknownTarget:
            std::cout << "Unknown actor: " << target << '\n';
            return;
        case ConnectionStatus::unknownBoth:
            std::cout << "Unknown actors: " << start << " and " << target << '\n';
            return;
        case ConnectionStatus::disconnected:
            std::cout << "No connection between " << start << " and " << target << " in this dataset.\n";
            return;
        case ConnectionStatus::found:
            break;
    }
    const auto count = result.steps.size();
    std::cout << "Shortest path: " << count << (count == 1 ? " connection" : " connections") << '\n';
    for (std::size_t i = 0; i < count; ++i) {
        const auto& step = result.steps[i];
        std::cout << i + 1 << ". " << step.fromActor << " -> " << step.movie.title;
        if (!step.movie.year.empty()) {
            std::cout << " (" << step.movie.year << ')';
        }
        std::cout << " -> " << step.toActor << '\n';
    }
}
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc > 2) {
        std::cerr << "Usage: AbsoluteCinema [movie-csv-path]\n";
        return 2;
    }
    const std::string filename = argc == 2 ? argv[1] : "IMDb movies.csv";
    try {
        std::cout << "Loading " << filename << "...\n" << std::flush;
        const Graph graph = DataFormat::loadIMDbData(filename);
        std::cout << "Loaded " << graph.movieCount() << " movies and " << graph.actorCount()
                  << " actor names.\nEnter names as credited, or type quit to exit.\n";
        std::string start;
        std::string target;
        while (readActor("\nFirst actor: ", start) && readActor("Second actor: ", target)) {
            printConnection(graph.shortestConnection(start, target), start, target);
        }
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}

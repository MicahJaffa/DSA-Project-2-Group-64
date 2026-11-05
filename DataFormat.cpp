#include "DataFormat.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
bool readRecord(std::istream& input, std::vector<std::string>& fields) {
    fields.clear();
    std::string line;
    if (!std::getline(input, line)) {
        return false;
    }
    fields.emplace_back();
    bool quoted = false;
    for (;;) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        for (std::size_t i = 0; i < line.size(); ++i) {
            const char c = line[i];
            if (c == '"') {
                if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                    fields.back() += '"';
                    ++i;
                } else {
                    quoted = !quoted;
                }
            } else if (c == ',' && !quoted) {
                fields.emplace_back();
            } else {
                fields.back() += c;
            }
        }
        if (!quoted) {
            return true;
        }
        fields.back() += '\n';
        if (!std::getline(input, line)) {
            throw std::runtime_error("Unclosed quoted field in the CSV.");
        }
    }
}

std::size_t columnIndex(const std::vector<std::string>& header, const std::string& name) {
    const auto match = std::find(header.begin(), header.end(), name);
    if (match == header.end()) {
        throw std::runtime_error("Missing CSV column '" + name + "'.");
    }
    return static_cast<std::size_t>(match - header.begin());
}

std::vector<std::string> splitCast(const std::string& text) {
    std::istringstream input(text);
    std::vector<std::string> cast;
    std::string name;
    while (std::getline(input, name, ',')) {
        name = DataFormat::trim(name);
        if (!name.empty()) {
            cast.push_back(name);
        }
    }
    return cast;
}
}

Graph DataFormat::loadIMDbData(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        throw std::runtime_error("Could not open '" + filename + "'.");
    }

    std::vector<std::string> header;
    if (!readRecord(input, header)) {
        throw std::runtime_error("The CSV is missing its header.");
    }
    if (header.front().compare(0, 3, "\xEF\xBB\xBF") == 0) {
        header.front().erase(0, 3);
    }
    const auto id = columnIndex(header, "imdb_title_id");
    const auto title = columnIndex(header, "title");
    const auto year = columnIndex(header, "year");
    const auto actors = columnIndex(header, "actors");

    Graph graph;
    std::vector<std::string> fields;
    std::size_t record = 1;
    while (readRecord(input, fields)) {
        ++record;
        if (fields.size() == 1 && trim(fields.front()).empty()) {
            continue;
        }
        if (fields.size() != header.size()) {
            throw std::runtime_error("Wrong number of fields in CSV record " + std::to_string(record) + ".");
        }
        Movie movie{trim(fields[id]), trim(fields[title]), trim(fields[year])};
        if (movie.imdbId.empty() || movie.title.empty()) {
            throw std::runtime_error("Missing movie ID or title in CSV record " + std::to_string(record) + ".");
        }
        graph.addMovie(movie, splitCast(fields[actors]));
    }
    if (input.bad()) {
        throw std::runtime_error("Error reading '" + filename + "'.");
    }
    if (graph.actorCount() == 0) {
        throw std::runtime_error("The CSV contains no actor credits.");
    }
    return graph;
}

std::string DataFormat::trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

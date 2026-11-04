#include <iostream>
#include <fstream>
#include "DataFormat.h"
void DataFormat::loadIMDbData(const std::string& filename, Graph& graph) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::vector<std::string> columns = parseCSVLine(line);

		if (columns.size() > 13) {
			std::string title = trim(columns[2]);
			std::string actorsStr = columns[13];

			std::vector<std::string> actors = splitActors(actorsStr);

			for (const std::string& actor : actors) {
				if (!actor.empty() && !title.empty()) {
					graph.insertEdge(actor, title);
				}
			}
		}
	}

	file.close();
}
std::vector<std::string> DataFormat::parseCSVLine(const std::string& line) {
	std::vector<std::string> result;
	std::string current;
	bool inQuotes = false;
	for (char c : line) {
		if (c == '"') {
			inQuotes = !inQuotes;
		}
		else if (c == ',' && !inQuotes) {
			result.push_back(current);
			current.clear();
		}
		else {
			current += c;
		}
	}
	result.push_back(current);
	return result;
}
 std::vector<std::string> DataFormat::splitActors(const std::string& actorsStr) {
	std::vector<std::string> actors;
	std::string current;
	for (char c : actorsStr) {
		if (c == ',') {
			actors.push_back(trim(current));
			current.clear();
		}
		else {
			current += c;
		}
	}
	if (!current.empty()) {
		actors.push_back(trim(current));
	}
	return actors;
}
std::string DataFormat::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}
/**
 * @file Leaderboard.cpp
 * @brief Class for handling the game leaderboard
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Leaderboard.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

bool Leaderboard::createSave(const std::string &filename) {
	std::ofstream outputFile(filename, std::ios::binary);
	if (!outputFile) {
		return false;
	}
	outputFile.close();
	return true;
}

template<typename T>
bool Leaderboard::read(std::ifstream &stream, T &value) {
	stream.read(reinterpret_cast<char *>(&value), sizeof(value));
	return stream.good();
}

template<typename T>
void Leaderboard::write(std::ofstream &stream, const T &value) {
	if (!stream.write(reinterpret_cast<const char *>(&value), sizeof(value))) {
		// wtf syntax
		throw std::runtime_error(&"Failed to write to file: " [ stream.tellp()]);
	}
}

bool Leaderboard::loadScores(const std::string &filename) {
	std::ifstream inputFile(filename, std::ios::binary);

	if (!inputFile) {
		// Create a new file if it does not exist.
		if (!createSave(filename)) {
			throw std::runtime_error("Failed to create save: " + filename);
		}
		return true;
	}

	scores.clear();

	uint32_t numEntries;
	if (!read(inputFile, numEntries)) {
		throw std::runtime_error("Failed to read number of entries from file: " + filename);
	}

	for (uint32_t i = 0; i < numEntries; ++i) {
		uint32_t nameLength;
		if (!read(inputFile, nameLength)) {
			throw std::runtime_error("Failed to read player name length from file: " + filename);
		}

		if (nameLength > 100) {
			throw std::runtime_error("Failed to read player name length from file: " + filename);
		}

		std::string playerName(nameLength + 1, '\0');
		if (!inputFile.read(&playerName[0], nameLength)) {
			throw std::runtime_error("Failed to read player name from file: " + filename);
		}

		uint32_t score;
		if (!read(inputFile, score)) {
			throw std::runtime_error("Failed to read player score from file: " + filename);
		}

		scores[playerName] = score;
	}

	return true;
}

bool Leaderboard::saveScores(const std::string &filename) {
	std::ofstream outputFile(filename, std::ios::binary);
	if (!outputFile) {
		throw std::runtime_error("Failed to open file for writing: " + filename);
	}

	uint32_t numEntries = scores.size();
	write(outputFile, numEntries);

	for (const auto &entry: scores) {
		uint32_t nameLength = entry.first.length();
		write(outputFile, nameLength);
		if (!outputFile.write(entry.first.c_str(), nameLength)) {
			throw std::runtime_error("Failed to write player name to file: " + filename);
		}
		write(outputFile, entry.second);
	}

	return true;
}

void Leaderboard::addScore(const std::string &player, unsigned score) {
	auto it = scores.find(player);
	if (it != scores.end()) {
		scores.erase(it);
	}
	scores[player] = score;
}

std::vector<std::pair<std::string, unsigned>> Leaderboard::getTopScores(unsigned count) {
	std::vector<std::pair<std::string, unsigned>> topScores(scores.begin(), scores.end());

	std::sort(topScores.begin(), topScores.end(), [](const auto &a, const auto &b) {
		return a.second > b.second;
	});

	while (topScores.size() < count) {
		topScores.emplace_back("-", 0);
	}

	topScores.resize(count);
	return topScores;
}





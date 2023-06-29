/**
 * @file Map.cpp
 * @brief Class for handling the game map
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Map.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <deque>

bool Map::isCrossroad(unsigned x, unsigned y) {
	if ((x >= (ghostCageBounds.xMin) && x <= (ghostCageBounds.xMax)) &&
		(y >= (ghostCageBounds.yMin) && y <= (ghostCageBounds.yMax))) {
		return false;
	}

	int accessibleDirections = 0;

	// Check all four directions if they are accessible
	for (auto [dx, dy]: directions) {
		unsigned newX = x + dx;
		unsigned newY = y + dy;

		if (newY < map.size() && newX < map[0].size() && map[newY][newX] != '#' && map[newY][newX] != 'X' &&
			map[newY][newX] != '=') {
			unsigned nextX = newX + dx;
			unsigned nextY = newY + dy;
			if (nextY < map.size() && nextX < map[0].size() && map[nextY][nextX] != '#' && map[nextY][nextX] != 'X' &&
				map[newY][newX] != '=') {
				++accessibleDirections;
			}
		}
	}

	return accessibleDirections >= 3;
}

void Map::findCrossroads() {
	// Find crossroads
	for (unsigned y = 0; y < map.size(); y++) {
		for (unsigned x = 0; x < map[0].size(); x++) {
			if (map[y][x] != '#' && map[y][x] != 'X' && map[y][x] != '=' && isCrossroad(x, y)) {
				crossroads.emplace_back(x, y);
			}
		}
	}
}

bool Map::loadMap(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + filename);
	}

	std::string mapString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return processMap(mapString);
}

bool Map::loadFromString(const std::string &mapString) {
	return processMap(mapString);
}

bool Map::traversableBFS(const std::pair<int, int> &start) {
	std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
	std::deque<std::pair<int, int>> queue;
	queue.push_back(start);
	visited[start.second][start.first] = true;

	while (!queue.empty()) {
		auto node = queue.front();
		queue.pop_front();

		for (auto &dir: directions) {
			int newX = node.first + dir.first;
			int newY = node.second + dir.second;

			if (newX >= 0 && newY >= 0 && newX < (int) map[0].size() && newY < (int) map.size() &&
				!visited[newY][newX] && map[newY][newX] != '#' && map[newY][newX] != 'X' && map[newY][newX] != '/') {
				queue.emplace_back(newX, newY);
				visited[newY][newX] = true;
			}
		}
	}

	// Check if all accessible cells are visited
	for (unsigned y = 0; y < map.size(); ++y) {
		for (unsigned x = 0; x < map[y].size(); ++x) {
			if (map[y][x] != '#' && map[y][x] != 'X' && map[y][x] != '/' && !visited[y][x]) {
				return false;
			}
		}
	}

	return true;
}

void Map::validateBorder(const std::string &cleanedLine) {
	if (teleports.size() % 2 != 0) {
		throw std::runtime_error("Invalid map: Wrong number of teleports.");
	}

	if (cleanedLine.empty() || (cleanedLine.front() != '#' && cleanedLine.front() != 'T') ||
		(cleanedLine.back() != '#' && cleanedLine.back() != 'T')) {
		throw std::runtime_error("Invalid map: Border must only contain walls or teleports.");
	}

	// Check for teleport on the opposite border
	auto validateTeleport = [&](char pos) {
		if (pos == 'T') {
			if (cleanedLine.front() != 'T' || cleanedLine.back() != 'T') {
				throw std::runtime_error("Invalid map: Missing matching teleport on the opposite border.");
			}
		}
	};

	// Validate teleports at the beginning and end of the line
	validateTeleport(cleanedLine.front());
	validateTeleport(cleanedLine.back());
}

bool Map::processMap(const std::string &mapString) {
	ghostCageBounds = {std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::min(),
					   std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::min()};

	bool foundGhostCage = false;
	bool foundGhostCageExit = false;

	std::istringstream ss(mapString);
	if (ss.peek() == std::istringstream::traits_type::eof()) {
		throw std::runtime_error("Empty map provided.");
	}

	std::string line;
	bool foundPlayer = false;
	int ghostCnt = 0;

	std::pair<unsigned, unsigned> playerStart;

	unsigned y = 0;
	size_t maxRowLength = 0;
	while (std::getline(ss, line)) {
		std::vector<char> row;
		unsigned x = 0;
		for (char c: line) {
			if (c != '@' && c != '&' && c != '.' && c != 'o' && c != 'T' && c != '#' && c != 'X' && c != '=' &&
				c != ' ' && c != '/') {
				throw std::runtime_error("Invalid character in map: " + std::string(1, c));
			}

			if (c == '@') {
				foundPlayer = true;
				playerStart = {x, y};
			} else if (c == '&') {
				ghostCnt++;
			} else if (c == '#' || c == '/') {
				walls.emplace_back(x, y);
			} else if (c == 'X') {
				foundGhostCage = true;
				ghostCageBounds.xMin = std::min(ghostCageBounds.xMin, x);
				ghostCageBounds.xMax = std::max(ghostCageBounds.xMax, x);
				ghostCageBounds.yMin = std::min(ghostCageBounds.yMin, y);
				ghostCageBounds.yMax = std::max(ghostCageBounds.yMax, y);
				walls.emplace_back(x, y);
			} else if (c == '=') {
				foundGhostCageExit = true;
				walls.emplace_back(x, y);
				ghostCageExits.emplace_back(x, y);
			} else if (c == 'T') {
				teleports.emplace_back(x, y);
			}

			row.push_back(c);

			++x;
			if (x > 28) throw std::runtime_error("Invalid map width: " + std::to_string(x));
		}

		if (y == 0)
			maxRowLength = std::max(maxRowLength, row.size());
		if (row.size() < maxRowLength || row.size() > maxRowLength) {
			throw std::runtime_error("Invalid map: empty/incomplete row.");
		}
		map.push_back(row);
		++y;
		if (y > 31) throw std::runtime_error("Invalid map height: " + std::to_string(y));
	}

	// Find crossroads
	findCrossroads();

	if (!foundPlayer || ghostCnt > 4 || ghostCnt < 1) {
		throw std::runtime_error("The map is not valid. No player or too many/few ghosts.");
	}

	if (!foundGhostCage) {
		throw std::runtime_error("No ghost cage found in the map.");
	}

	if (!foundGhostCageExit) {
		throw std::runtime_error("No exit found for the ghost cage in the map.");
	}

	for (auto &exit: ghostCageExits) {
		if (!((exit.first == ghostCageBounds.xMin || exit.first == ghostCageBounds.xMax) &&
			  (exit.second >= ghostCageBounds.yMin && exit.second <= ghostCageBounds.yMax)) &&
			!((exit.second == ghostCageBounds.yMin || exit.second == ghostCageBounds.yMax) &&
			  (exit.first >= ghostCageBounds.xMin && exit.first <= ghostCageBounds.xMax))) {
			throw std::runtime_error("Ghost cage exit not on the cage border.");
		}
	}

	// Check each row's border
	for (auto &row: map) {
		std::string cleanedRow;
		for (auto c: row) {
			if (c != '/') {
				cleanedRow.push_back(c);
			}
		}
		validateBorder(cleanedRow);
	}

	// Check each column's border
	for (unsigned i = 0; i < maxRowLength; ++i) {
		std::string cleanedColumn;
		for (auto &row: map) {
			if (row[i] != '/') {
				cleanedColumn.push_back(row[i]);
			}
		}
		validateBorder(cleanedColumn);
	}

	if (!traversableBFS(playerStart)) {
		throw std::runtime_error("The map is not traversable.");
	}

	return true;
}

bool Map::saveMap(const std::string &filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open map file for writing: " + filename);
	}

	for (const auto &row: map) {
		for (char c: row) {
			file << c;
		}
		file << '\n';
	}

	file.close();
	return true;
}

void Map::clearMap() {
	map.clear();
	crossroads.clear();
	walls.clear();
	ghostCageExits.clear();
}

bool Map::isCrossroadAt(unsigned x, unsigned y) {
	return std::any_of(crossroads.begin(), crossroads.end(), [&](const auto &crossroad) {
		return crossroad.first == x && crossroad.second == y;
	});
}

bool Map::isWallAt(unsigned x, unsigned y) {
	return std::any_of(walls.begin(), walls.end(), [&](const auto &wall) {
		return wall.first == x && wall.second == y;
	});
}

bool Map::isInGhostCage(unsigned x, unsigned y) const {
	return x >= ghostCageBounds.xMin && x <= ghostCageBounds.xMax && y >= ghostCageBounds.yMin &&
		   y <= ghostCageBounds.yMax;
}

std::pair<unsigned, unsigned> Map::getNearestGhostCageExit(unsigned x, unsigned y) const {
	std::pair<unsigned, unsigned> nearestExit = ghostCageExits[0];
	int minDistance = (x - nearestExit.first) + (y - nearestExit.second);

	for (const auto &exit: ghostCageExits) {
		int distance = (x - exit.first) + (y - exit.second);
		if (distance < minDistance) {
			minDistance = distance;
			nearestExit = exit;
		}
	}

	return nearestExit;
}

std::pair<unsigned, unsigned> Map::getSize() const {
	return {map[0].size(), map.size()};
}
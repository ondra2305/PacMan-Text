/**
 * @file Map.h
 * @brief Class for handling the game map
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_MAP_H
#define SEMESTRAL_WORK_MAP_H

#include <string>
#include <vector>
#include "Window.h"

class Map {
public:
	Map() = default;

	Map(const Map &other) = default;

	~Map() = default;

	/**
	 * @brief Loads the map from a file
	 * @param filename the file to load from
	 * @return true if successful, false otherwise
	 */
	bool loadMap(const std::string &filename);

	/**
	 * @brief Loads the map from a string
	 * @param mapString the string to load from
	 * @return true if successful, false otherwise
	 */
	bool loadFromString(const std::string &mapString);

	/**
	 * @brief Gets the map
	 * @return the map
	 */
	std::vector<std::vector<char>> &getMap() { return map; }

	/**
	 * @brief Saves the map to a file
	 * @param filename
	 * @return true if successful, false otherwise
	 */
	bool saveMap(const std::string &filename);

	/**
	 * @brief Clears the map
	 */
	void clearMap();

	/**
	 * @brief Checks if there is a crossroad at the given coordinates
	 * @param x coordinate
	 * @param y coordinate
	 * @return true if there is a crossroad
	 */
	bool isCrossroadAt(unsigned x, unsigned y);

	/**
	 * @brief Checks if there is a wall at the given coordinates
	 * @param x coordinate
	 * @param y coordinate
	 * @return true if there is a wall
	 */
	bool isWallAt(unsigned x, unsigned y);

	/**
	 * @brief Checks if given position is in ghost cage
	 * @param x coordinate
	 * @param y coordinate
	 * @return true if position is in ghost cage
	 */
	[[nodiscard]] bool isInGhostCage(unsigned x, unsigned y) const;

	/**
	 * @brief Gets the nearest ghost cage exit
	 * @param x from x coordinate
	 * @param y from y coordinate
	 * @return pair of coordinates of the nearest ghost cage exit
	 */
	[[nodiscard]] std::pair<unsigned, unsigned> getNearestGhostCageExit(unsigned x, unsigned y) const;

	/**
	 * @brief Gets the map size
	 * @return pair of map size (width, height)
	 */
	[[nodiscard]] std::pair<unsigned, unsigned> getSize() const;

private:
	std::vector<std::vector<char>> map;
	std::vector<std::pair<unsigned, unsigned>> crossroads;
	std::vector<std::pair<unsigned, unsigned>> walls;
	std::vector<std::pair<unsigned, unsigned>> teleports;
	struct Bounds {
		unsigned xMin;
		unsigned xMax;
		unsigned yMin;
		unsigned yMax;
	};

	Bounds ghostCageBounds;
	std::vector<std::pair<unsigned, unsigned>> ghostCageExits;

	std::vector<std::pair<int, int>> directions = {{-1, 0}, // left
												   {1,  0}, // right
												   {0,  -1}, // up
												   {0,  1} // down
	};

	/**
	 * @brief Checks if given coordinates are a crossroad
	 * @param x position
	 * @param y position
	 * @return true if coordinates are a crossroad
	 */
	bool isCrossroad(unsigned x, unsigned y);

	/**
	 * @brief Helper function for finding crossroads
	 */
	void findCrossroads();

	/**
	 * @brief Helper function for validating map string
	 * @param mapString
	 * @return true if map string is valid
	 */
	bool processMap(const std::string &mapString);

	/**
	 * @brief Checks if the map is traversable from the given position
	 * @param start position
	 * @return true if the map is traversable
	 */
	bool traversableBFS(const std::pair<int, int> &start);

	/**
	 * @brief Helper function for validating the map borders
	 * @param cleanedLine one row or column of the map
	 */
	void validateBorder(const std::string &cleanedLine);
};

#endif //SEMESTRAL_WORK_MAP_H

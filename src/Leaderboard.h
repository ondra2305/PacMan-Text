/**
 * @file Leaderboard.h
 * @brief Class for handling the game leaderboard
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_LEADERBOARD_H
#define SEMESTRAL_WORK_LEADERBOARD_H

#include <vector>
#include <string>
#include <map>
#include <cstdint>

class Leaderboard {
public:
	Leaderboard() = default;

	~Leaderboard() = default;

	/**
	 * @brief Loads the scores from a file
	 * @param filename the file to load from
	 * @return true if successful, false otherwise
	 */
	bool loadScores(const std::string &filename);

	/**
	 * @brief Saves the scores to a file
	 * @param filename the file to save to
	 * @return true if successful, false otherwise
	 */
	bool saveScores(const std::string &filename);

	/**
	 * @brief Adds a score to the leaderboard
	 * @param player the player's name
	 * @param score the player's score
	 */
	void addScore(const std::string &player, unsigned score);

	/**
	 * @brief Gets the top scores
	 * @param count the number of scores to get
	 * @return the top scores
	 */
	std::vector<std::pair<std::string, unsigned>> getTopScores(unsigned count);

private:
	std::map<std::string, uint32_t> scores;

	/**
	 * @brief Creates a save file
	 * @param filename the file to save to
	 * @return true if successful, false otherwise
	 */
	static bool createSave(const std::string &filename);

	/**
	 * @brief Reads a value from binary save file
	 * @tparam T the type of the value
	 * @param stream the file stream
	 * @param value the value to read
	 * @return true if successful, false otherwise
	 */
	template<typename T>
	bool read(std::ifstream &stream, T &value);

	/**
	 * @brief Writes a value to binary save file
	 * @tparam T the type of the value
	 * @param stream the file stream
	 * @param value the value to write
	 */
	template<typename T>
	void write(std::ofstream &stream, const T &value);
};

#endif //SEMESTRAL_WORK_LEADERBOARD_H

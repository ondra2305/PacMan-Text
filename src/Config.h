/**
 * @file Config.h
 * @brief Class for handling the game configuration
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_CONFIG_H
#define SEMESTRAL_WORK_CONFIG_H

#include <string>
#include <map>
#include <vector>

class Config {
public:
	Config() = default;

	Config(const Config &other) = default;

	/**
	 * @brief Copy constructor for configs
	 * @param confName config name
	 * @param map map file path
	 * @param saveFile save file path
	 */
	Config(const std::string &confName, const std::string &map, const std::string &saveFile);

	~Config() = default;

	/**
	 * @brief Loads the config from a file
	 * @param filename the file to load from
	 * @return true if successful, false otherwise
	 */
	bool loadConfig(const std::string &filename);

	/**
	 * @brief Exports the config to a file
	 * @param filename the file to export to
	 * @return true if successful, false otherwise
	 */
	bool exportConfig(const std::string &filename);

	/**
	 * @brief Gets any value from the config map
	 * @return the wanted value
	 */
	[[nodiscard]] std::string getValue(const std::string &key) const;

	/**
	 * @brief Gets the map file path
	 * @return the map file path
	 */
	[[nodiscard]] std::string getPath() const;

private:
	std::map<std::string, std::string> configMap;
	std::string filePath;
	//int mazeHeight;
	//int mazeWidth;

	std::vector<std::string> allowedKeys = {"configName", "map", "saveFile", "ghostSpeed", "pacManSpeed",
											"ghostHuntingModeDuration", "bonusProbability", "amountOfLives"};

	std::map<std::string, std::string> defaultConfigMap = {{"ghostSpeed",               "1.0"},
														   {"pacManSpeed",              "1.0"},
														   {"ghostHuntingModeDuration", "10"},
														   {"bonusProbability",         "0.1"},
														   {"amountOfLives",            "3"}};
};

#endif //SEMESTRAL_WORK_CONFIG_H

/**
 * @file Config.cpp
 * @brief Class for handling the game configuration
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Config.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Map.h"
#include <filesystem>

Config::Config(const std::string &confName, const std::string &map, const std::string &saveFile) {
	configMap["configName"] = confName;
	configMap["map"] = map;
	configMap["saveFile"] = saveFile;

	// Add missing keys
	for (const auto &[key, value]: defaultConfigMap) {
		configMap.try_emplace(key, value);
	}

	filePath = confName;
}

bool Config::loadConfig(const std::string &filename) {
	filePath = filename;
	std::ifstream file(filename + "/Config.txt");
	std::string line;

	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file: " + filename);
	}

	while (std::getline(file, line)) {
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '=')) {
			std::string value;
			if (std::getline(is_line, value)) {
				if (key.empty()) {
					throw std::runtime_error("Missing key in config: " + filename);
				}

				if (value.empty()) {
					throw std::runtime_error("Missing value for key in config: " + key);
				}

				if (std::find(allowedKeys.begin(), allowedKeys.end(), key) == allowedKeys.end()) {
					throw std::runtime_error("Unknown key in config: " + filename);
				}

				if (configMap.find(key) != configMap.end()) {
					throw std::runtime_error("Duplicate key in config: " + key);
				}

				if (key == "ghostSpeed" || key == "pacManSpeed") {
					try {
						double v = std::stod(value);
						if (v < 0.5 || v > 5.0) {
							throw std::runtime_error("Invalid value for key in config: " + key + ". Value must be between 0.5 and 5.0");
						}
					} catch (const std::invalid_argument &) {
						throw std::runtime_error("Invalid value for key in config: " + key + ".");
					}
				} else if (key == "bonusProbability") {
					try {
						double v = std::stod(value);
						if (v < 0 || v > 1) {
							throw std::runtime_error("Invalid value for key in config: " + key + ". Value must be between 0 and 1");
						}
					} catch (const std::invalid_argument &) {
						throw std::runtime_error("Invalid value for key in config: " + key + ".");
					}
				} else if (key == "ghostHuntingModeDuration") {
					try {
						int v = std::stoi(value);
						if (v < 5 || v > 30) {
							throw std::runtime_error("Invalid value for key in config: " + key + ". Value must be between 5 and 30");
						}
					} catch (const std::invalid_argument &) {
						throw std::runtime_error("Invalid value for key in config: " + key + ".");
					}
				} else if (key == "amountOfLives") {
					try {
						int v = std::stoi(value);
						if (v <= 0) {
							throw std::runtime_error("Invalid value for key in config: " + key + ".");
						}
					} catch (const std::invalid_argument &) {
						throw std::runtime_error("Invalid value for key in config: " + key + ".");
					}
				}

				configMap[key] = value;
			} else {
				throw std::runtime_error("Invalid formatting in config. Missing value for key: " + key);
			}
		}
	}

	// Check if configName, map and saveFile exist in the configMap
	if (configMap.find("configName") == configMap.end() || configMap.find("map") == configMap.end() ||
		configMap.find("saveFile") == configMap.end()) {
		throw std::runtime_error("Required keys missing in config file: " + filename);
	}

	// Iterate over the defaultConfigMap and add missing keys
	for (const auto &[key, value]: defaultConfigMap) {
		configMap.try_emplace(key, value);
	}

	Map tmpMap;
	if (!tmpMap.loadMap(filename + "/" + getValue("map"))) {
		throw std::runtime_error("Invalid map file: " + filename);
	}

	return true;
}

bool Config::exportConfig(const std::string &directory) {
	// Create the output directory if it doesn't exist
	std::filesystem::create_directories(directory);

	std::string filename = directory + "/Config.txt";
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file: " + filename);
	}

	for (const auto &entry: configMap) {
		file << entry.first << '=' << entry.second << '\n';
	}

	return true;
}

std::string Config::getValue(const std::string &key) const {
	auto it = configMap.find(key);
	if (it == configMap.end()) {
		throw std::runtime_error("Key not found in config: " + key);
	}
	return it->second;
}

std::string Config::getPath() const {
	return filePath;
}

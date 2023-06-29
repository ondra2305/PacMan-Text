/**
 * @file GameManager.cpp
 * @brief Handles the game loop and game states
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "GameManager.h"
#include <ncurses.h>
#include <iostream>
#include "Enums.h"
#include <chrono>
#include <thread>
#include <unistd.h>

GameManager::GameManager() : window() {
	gameState = GameState::InMenu;
	confDir = "";
}

void GameManager::start() {
	window.init();
	gameState = GameState::InMenu;

	const std::chrono::milliseconds frameTime(1000 / 60);
	auto lastFrameTime = std::chrono::steady_clock::now();

	while (true) {
		auto frameStartTime = std::chrono::steady_clock::now();

		if (gameState == GameState::Playing) {
			std::chrono::duration<float, std::milli> frameDuration = frameStartTime - lastFrameTime;
			float deltaTime = frameDuration.count() / 1000.0f;  // convert milliseconds to seconds

			handleInput();
			if (game) {
				try {
					game->update(gameState, deltaTime);
				} catch (std::exception &e) {
					gameState = GameState::Exit;
					window.printMessage(e.what(), false);
					handleInput();
				}
			}
		} else if (gameState == GameState::Paused) {
			handleInput();
			gameState = GameState::Playing;
		} else if (gameState == GameState::GameOver) {
			if (game) game->end();
			handleInput();
			gameState = GameState::InMenu;
		} else if (gameState == GameState::Exit) {
			window.exit();
			break;  // Break the game loop
		} else {
			int userChoice = window.getChoice();

			switch (userChoice) {
				case 0: // Start New Game
					game = std::make_unique<Game>(window);
					gameState = GameState::Playing;
					game->startNew(gameState, confDir);
					break;
				case 1: // Load Config file
				{
					Config config;
					std::string configName = window.getInput("Enter config path:");
					try {
						config.loadConfig(confDir + configName);
					} catch (std::exception &e) {
						window.printMessage(e.what(), true);
						gameState = GameState::InMenu;
						break;
					}
					game = std::make_unique<Game>(window);
					gameState = GameState::Playing;
					game->start(config, gameState);
					break;
				}
				case 2: // Load Only Map
				{
					Map map;
					std::string mapName = window.getInput("Enter map path:");
					try {
						map.loadMap(confDir + mapName);
					} catch (std::exception &e) {
						window.printMessage(e.what(), true);
						gameState = GameState::InMenu;
						break;
					}
					game = std::make_unique<Game>(window);
					gameState = GameState::Playing;
					game->startNew(gameState, confDir, map);
					break;
				}
				case 3:
				{
					confDir = "";
					confDir = window.getInput("Enter config directory:");
					if (confDir.back() != '/') {
						confDir += '/';
					}

					// check if the directory exists and is writable
					try {
						if (access(confDir.c_str(), F_OK | W_OK) == -1) {
							throw std::runtime_error(confDir + " is not a valid directory or it is not writable");
						}
					} catch (const std::runtime_error& e) {
						window.printMessage(e.what(), true);
					}
					break;
				}
				case 4: // Help
					gameState = GameState::InMenu;
					window.printMessage("Controls:\n* Arrows/WASD - movement\n* ESC - pause\n* Q - quit\n"
										"Tips:\n"
										"* when loading a Save, input full path to the savefile folder\n"
										"* when loading only a map, input full path to the map (including .txt)\n"
										"Credits:\n"
										"Ondrej Svoboda (svoboo30), FIT CVUT", false);
					break;
				case 5: // Exit
					gameState = GameState::Exit;
					break;
				default:
					break;
			}
		}

		auto frameEndTime = std::chrono::steady_clock::now();
		std::chrono::duration<float, std::milli> frameDuration = frameEndTime - frameStartTime;

		if (frameDuration < frameTime) {
			std::this_thread::sleep_for(frameTime - frameDuration);
		}

		lastFrameTime = frameEndTime;
	}
}

void GameManager::handleInput() {
	// Enable non-blocking mode
	if (gameState == GameState::Playing)
		nodelay(stdscr, true);
	else
		nodelay(stdscr, false);

	if (!game)
		throw std::runtime_error("ERROR! Game is null!");

	int ch = getch();
	if (ch != ERR) {
		switch (toupper(ch)) {
			case KEY_UP:
			case 'W':
				game->movePlayer(Direction::UP);
				break;
			case KEY_DOWN:
			case 'S':
				game->movePlayer(Direction::DOWN);
				break;
			case KEY_LEFT:
			case 'A':
				game->movePlayer(Direction::LEFT);
				break;
			case KEY_RIGHT:
			case 'D':
				game->movePlayer(Direction::RIGHT);
				break;
			case 'Q':
				gameState = GameState::GameOver;
				break;
			case 27 : //ESC
				gameState = GameState::Paused;
				break;
			default:
				break;
		}
	}
}

/**
 * @file Game.cpp
 * @brief Class for handling the game logic
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Game.h"
#include <memory>
#include <algorithm>
#include <iostream>
#include "Config.h"
#include "EnemyBehavior.h"
#include "ScatterBehavior.h"
#include "Cherry.h"
#include "Coin.h"
#include "Teleport.h"
#include "Wall.h"
#include "Helpers.h"

void Game::initializeGame() {
	gameWindow.clearCurrentWin();
	std::string mapPath = gameConfig.getPath() + "/" + gameConfig.getValue("map");

	gameWindow.printMessage(("Loading map: " + mapPath + "\n<Press any key>"), false);
	gameMap.loadMap(mapPath);

	gamePhase.setPhase(GamePhaseState::NotStarted);
	loadObjects();

	std::string playerName;
	while(playerName.length() >= 33 || playerName.length() <= 0 || playerName.empty()) {
		playerName = gameWindow.getInput("Enter your nickname:");
		if(playerName.length() > 32) {
			gameWindow.printMessage("Nickname too long/short!", true);
		}
	}
	player->setNickname(playerName);

	gameWindow.clearCurrentWin();
	gameLeaderboard.loadScores(gameConfig.getPath() + "/" + gameConfig.getValue("saveFile"));
	gameWindow.printMessage("Game started!\n<Press any key>", false);
	gameWindow.clearCurrentWin();
	draw();
}

void Game::start(const Config &config, GameState &gameState) {
	gameConfig = config;
	try {
		initializeGame();
		gamePhase.setPhase(GamePhaseState::NotStarted);
	} catch (std::exception &e) {
		gameWindow.printMessage(e.what(), true);
		gameState = GameState::InMenu;
		endOnErr();
	}
}

void Game::startNew(GameState &gameState, const std::string& confPath, const std::optional<const Map> &map) {
	const std::string defaultMapString = "############################\n"
										 "#............##............#\n"
										 "#.####.#####.##.#####.####.#\n"
										 "#o#//#.#///#.##.#///#.#//#o#\n"
										 "#.####.#####.##.#####.####.#\n"
										 "#..........................#\n"
										 "#.####.##.########.##.####.#\n"
										 "#.####.##.########.##.####.#\n"
										 "#......##....##....##......#\n"
										 "######.##### ## #####.######\n"
										 "/////#.##### ## #####.#/////\n"
										 "/////#.##    &     ##.#/////\n"
										 "/////#.## XXX==XXX ##.#/////\n"
										 "######.## X      X ##.######\n"
										 "T     .   X      X   .     T\n"
										 "######.## X& & & X ##.######\n"
										 "/////#.## XXXXXXXX ##.#/////\n"
										 "/////#.##          ##.#/////\n"
										 "/////#.## ######## ##.#/////\n"
										 "######.## ######## ##.######\n"
										 "#............##............#\n"
										 "#.####.#####.##.#####.####.#\n"
										 "#.####.#####.##.#####.####.#\n"
										 "#o..##.......@........##..o#\n"
										 "###.##.##.########.##.##.###\n"
										 "###.##.##.########.##.##.###\n"
										 "#......##....##....##......#\n"
										 "#.##########.##.##########.#\n"
										 "#.##########.##.##########.#\n"
										 "#..........................#\n"
										 "############################\n";

	gameWindow.clearCurrentWin();
	gameWindow.printMessage("Starting new game...\n<Press any key>", false);
	try {
		if (map.has_value()) {
			gameMap = map.value();
		} else {
			gameMap.loadFromString(defaultMapString);
		}
		std::string wantedPath;
		if(confPath.empty())
			wantedPath = "examples/";
		else
			wantedPath = confPath;

		Config newConfig(wantedPath + "PacMan" + generateRandomNumber(6), "Map.txt", "Savefile");
		gameConfig = newConfig;
		gameConfig.exportConfig(gameConfig.getValue("configName"));
		gameMap.saveMap(gameConfig.getPath() + "/" + gameConfig.getValue("map"));
	} catch (std::exception &e) {
		gameWindow.printMessage(e.what(), true);
		gameState = GameState::InMenu;
		endOnErr();
	}
	gameMap.clearMap();
	try {
		initializeGame();
		gamePhase.setPhase(GamePhaseState::NotStarted);
	} catch (std::exception &e) {
		gameWindow.printMessage(e.what(), true);
		gameState = GameState::InMenu;
		endOnErr();
	}
}

void Game::loadObjects() {
	const auto &mapData = gameMap.getMap();
	int ghostCnt = 0;
	float delayLen = 0.0f;
	std::vector<std::pair<int, ColorCode>> ghostColors = {{0, COLOR_RED_CODE},
														  {1, COLOR_MAGENTA_CODE},
														  {2, COLOR_CYAN_CODE},
														  {3, COLOR_YELLOW_CODE}};

	for (size_t y = 0; y < mapData.size(); ++y) {
		for (size_t x = 0; x < mapData[y].size(); ++x) {
			switch (mapData[y][x]) {
				case '@':  // '@' represents player
					player = std::make_shared<Player>(x, y, std::stod(gameConfig.getValue("pacManSpeed")),
													  Direction::LEFT);
					player->setLives(std::stoi(gameConfig.getValue("amountOfLives")));
					player->setScore(0);
					break;
				case '&':  // '&' represents enemy
				{
					std::shared_ptr<Ghost> ghost;
					ghost = std::make_shared<Ghost>(x, y, std::stod(gameConfig.getValue("ghostSpeed")), Direction::UP,
													std::make_unique<ScatterBehavior>(), ghostColors[ghostCnt].second,
													delayLen);
					enemies.push_back(ghost);
					ghostCnt++;
					delayLen += 3.0f;
					break;
				}
				case '.':  // '.' represents coin
					objects.push_back(std::make_shared<Coin>(x, y));
					gamePhase.addItem();
					break;
				case 'o':  // 'o' represents cherry
					objects.push_back(std::make_shared<Cherry>(x, y));
					gamePhase.addItem();
					break;
				case 'T':  // 'T' represents teleport
					objects.push_back(std::make_shared<Teleport>(x, y));
					break;
				case '#':  // '#' represents wall
					objects.push_back(std::make_shared<Wall>(x, y, '#', COLOR_BLUE_CODE, false));
					break;
				case 'X':  // 'X' represents wall around ghost house
					objects.push_back(std::make_shared<Wall>(x, y, 'X', COLOR_BLUE_CODE, false));
					break;
				case '=':  // '=' represents ghost house exit
					objects.push_back(std::make_shared<Wall>(x, y, '=', COLOR_BLUE_CODE, true));
					break;
				default:
					break;
			}
		}
	}
}

void Game::draw() {
	for (auto &object: objects) {
		if (!object->isPickedUp())
			gameWindow.printCharAt(object->getX(), object->getY() + 2, object->getSymbol(), false, object->getColor());
		else
			gameWindow.printCharAt(object->getX(), object->getY() + 2, ' ', false, object->getColor());
	}
	for (auto &object: enemies) {
		gameWindow.printCharAt(object->getX(), object->getY() + 2, object->getSymbol(), false, object->getColor());
	}
	gameWindow.printCharAt(player->getX(), player->getY() + 2, player->getSymbol(), false, player->getColor());
	gameWindow.printUI(player->getScore(), player->getLives(), gamePhase.getLevel(), gamePhase.getRemainingItems());
	gameWindow.refreshWin();
}

void Game::update(GameState &gameState, float deltaTime) {
	if (gameState == GameState::Paused) {
		gameLeaderboard.addScore(player->getNickname(), player->getScore());
		gameWindow.displayLeaderboard(gameLeaderboard.getTopScores(5), false);
		return;
	} else if (gameState == GameState::GameOver) {
		return;
	} else if (gameState != GameState::Playing) {
		throw std::runtime_error("ERROR! Updating game while not playing!");
	}

	// Check if player died or completed level
	if (gamePhase.getPhase() == GamePhaseState::NotStarted) {
		updatePhaseTimer(deltaTime);
		return;
	} else if (gamePhase.getPhase() == GamePhaseState::PlayerDied) {
		returnAllToStart();
		gamePhase.setPhase(GamePhaseState::NotStarted);
	} else if (gamePhase.getPhase() == GamePhaseState::PlayerDiedEnd) {
		gameState = GameState::GameOver;
		end();
		return;
	} else if (gamePhase.getPhase() == GamePhaseState::LevelComplete) {
		gamePhase.setPhase(GamePhaseState::NotStarted);
		gamePhase.nextLevel();
		for (auto &object: objects) {
			if (object->isPickupable()) {
				object->reset();
				gamePhase.addItem();
			}
		}
		returnAllToStart();
		for (auto &enemy: enemies) {
			enemy->update(deltaTime, gamePhase);
		}
		player->update(deltaTime, gamePhase);
		gameWindow.clearCurrentWin();
		draw();
	}
	updatePhaseTimer(deltaTime);

	int playerPrevX = player->getX();
	int playerPrevY = player->getY();
	bool playerDied = false;
	bool playerMovement = true;

	// Check for collisions between enemies and player
	for (auto &enemy: enemies) {
		enemy->getNextMove(player, gameMap);
		if (checkCollisionAt(player, enemy->getX(), enemy->getY())) {
			if (!enemy->interactWith(player, gamePhase)) {
				playerDied = true;
			}
			gameWindow.printUI(player->getScore(), player->getLives(), gamePhase.getLevel(),
							   gamePhase.getRemainingItems());
			break;
		}
	}

	if (!playerDied) {
		if (!player->getNextMove(player, gameMap)) {
			player->resetDirection();
			playerMovement = false;
		} else {
			int playerNewX = player->getX();
			int playerNewY = player->getY();

			// Check collisions for the player
			for (auto &object: objects) {
				if (checkCollisionAt(object, playerNewX, playerNewY)) {
					if (!object->isAccessibleFor(player)) {
						player->resetDirection();
						playerMovement = false;
					} else {
						if (object->interactWith(player, gamePhase)) {
							gamePhase.removeItem();
						}
					}
					break;
				}
			}
		}
	}

	if (playerMovement && !playerDied) {
		player->update(deltaTime, gamePhase);
		gameWindow.printCharAt(player->getX(), player->getY() + 2, player->getSymbol(), false, player->getColor());
		gameWindow.printCharAt(playerPrevX, playerPrevY + 2, ' ', false);
	}

	// Update enemies and check for collisions with the environment
	for (auto &enemy: enemies) {
		int enemyPrevX = enemy->getX();
		int enemyPrevY = enemy->getY();

		if (enemy->getNextMove(player, gameMap)) {
			int enemyNewX = enemy->getX();
			int enemyNewY = enemy->getY();

			// Check collisions for the enemy
			for (auto &object: objects) {
				if (checkCollisionAt(object, enemyNewX, enemyNewY)) {
					if (!object->isAccessibleFor(enemy)) {
						enemy->getNextMove(player, gameMap);
					}
					object->interactWith(enemy, gamePhase);
					break;
				}
			}
		}

		enemy->update(deltaTime, gamePhase);
		gameWindow.printCharAt(enemy->getX(), enemy->getY() + 2, enemy->getSymbol(), false, enemy->getColor());
		gameWindow.printCharAt(enemyPrevX, enemyPrevY + 2, ' ', false);
	}

	if (playerDied)
		gameWindow.printCharAt(playerPrevX, playerPrevY + 2, ' ', false);

	draw();
}

void Game::movePlayer(Direction direction) {
	if (gamePhase.getPhase() == GamePhaseState::NotStarted)
		gamePhase.setPhase(GamePhaseState::Scatter);
	player->setDirection(direction);
}

bool Game::checkCollisionAt(const std::shared_ptr<GameObject> &object, int x, int y) {
	return object->getX() == x && object->getY() == y;
}

void Game::updatePhaseTimer(float deltaTime) {
	gamePhase.addPhaseTime(deltaTime);

	static float behaviorChangeInterval = 5.0f;
	static float behaviorMultiplier = 2.0f;

	switch (gamePhase.getPhase()) {
		case GamePhaseState::NotStarted:
			if (gamePhase.getPhaseTime() >= 3.0f) {
				gamePhase.setPhaseTime(0.0f);
				gamePhase.setPhase(GamePhaseState::Scatter);
			}
			break;

		case GamePhaseState::Scatter:
			if (gamePhase.getPhaseTime() >= behaviorChangeInterval / (float) gamePhase.getLevel()) {
				gamePhase.setPhase(GamePhaseState::Chase);
				gamePhase.setPhaseTime(0.0f);
			}
			break;

		case GamePhaseState::Chase:
			if (gamePhase.getPhaseTime() >=
				behaviorChangeInterval * behaviorMultiplier * (float) gamePhase.getLevel()) {
				gamePhase.setPhase(GamePhaseState::Scatter);
				gamePhase.setPhaseTime(0.0f);
			}
			break;

		case GamePhaseState::GhostHunting:
			if (gamePhase.getPhaseTime() >= std::stod(gameConfig.getValue("ghostHuntingModeDuration"))) {
				gamePhase.setPhase(GamePhaseState::Scatter);
				gamePhase.setPhaseTime(0.0f);
			}
			break;

		default:
			break;
	}
}

void Game::returnAllToStart() {
	gameWindow.printCharAt(player->getX(), player->getY() + 2, ' ', false);
	player->returnToStart();
	for (auto &enemy: enemies) {
		gameWindow.printCharAt(enemy->getX(), enemy->getY() + 2, ' ', false);
		enemy->returnToStart();
	}
	draw();
}

void Game::endOnErr() {
	gameMap.clearMap();
	gameWindow.clearCurrentWin();
	gameWindow.printMessage("Error occured! Game will exit.", true);
}

void Game::end() {
	gameMap.clearMap();
	gameWindow.clearCurrentWin();
	gameLeaderboard.addScore(player->getNickname(), player->getScore());
	gameWindow.displayLeaderboard(gameLeaderboard.getTopScores(5), true);
	try {
		gameLeaderboard.saveScores(gameConfig.getPath() + "/" + gameConfig.getValue("saveFile"));
	} catch (const std::exception &e) {
		gameWindow.printMessage(e.what(), true);
		endOnErr();
	}
}

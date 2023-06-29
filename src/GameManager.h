/**
 * @file GameManager.h
 * @brief Handles the game loop and game states
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_GAMEMANAGER_H
#define SEMESTRAL_WORK_GAMEMANAGER_H

#include "Game.h"
#include "Window.h"
#include "Enums.h"

class GameManager {
public:
	GameManager();

	~GameManager() = default;

	void start();

	void handleInput();

private:
	Window window;
	std::unique_ptr<Game> game;
	GameState gameState;
	std::string confDir;
};

#endif //SEMESTRAL_WORK_GAMEMANAGER_H

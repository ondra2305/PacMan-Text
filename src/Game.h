/**
 * @file Game.h
 * @brief Class for handling the game logic
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_GAME_H
#define SEMESTRAL_WORK_GAME_H

#include <memory>
#include <vector>
#include "GameObject.h"
#include "Config.h"
#include "Window.h"
#include "Player.h"
#include "Ghost.h"
#include "Map.h"
#include "Leaderboard.h"
#include "InteractiveObject.h"
#include "GamePhase.h"

class Game {
public:
	explicit Game(Window &window) : gameWindow(window) {}

	~Game() = default;

	/**
	 * @brief Starts the game
	 * @param config the game configuration
	 * @param gameState the game state
	 */
	void start(const Config &config, GameState &gameState);

	/**
	 * @brief Starts the game with a new map
	 * @param gameState the game state
	 * @param confPath config folder path
	 * @param map the new map
	 */
	void startNew(GameState &gameState, const std::string& confPath, const std::optional<const Map> &map = std::nullopt);

	/**
	 * @brief Ends the game
	 */
	void end();

	/**
	 * @brief Ends the game on error
	 */
	void endOnErr();

	/**
	 * @brief Updates the game state
	 * @param state the game state
	 * @param deltaTime the time since last update
	 */
	void update(GameState &state, float deltaTime);

	/**
	 * @brief Moves the player in given direction
	 * @param direction the direction to move
	 */
	void movePlayer(Direction direction);

private:
	Config gameConfig;
	Window &gameWindow;
	Map gameMap;
	Leaderboard gameLeaderboard;
	GamePhase gamePhase;

	std::shared_ptr<MovingObject> player;
	std::vector<std::shared_ptr<MovingObject>> enemies;
	std::vector<std::shared_ptr<InteractiveObject>> objects;

	/**
	 * @brief Loads the object from the map
	 * @return void
	 */
	void loadObjects();

	/**
	 * @brief Draws the game
	 * @return void
	 */
	void draw();

	/**
	 * @brief Checks for collisions at given position for given object
	 * @param object which object to check for
	 * @param x position to check
	 * @param y position to check
	 * @return true if there is a collision
	 */
	static bool checkCollisionAt(const std::shared_ptr<GameObject> &object, int x, int y);

	/**
	 * @brief Initializes the game, loading config, and the map
	 */
	void initializeGame();

	/**
	 * @brief Updates the game phase timer
	 * @param deltaTime
	 * @return void
	 */
	void updatePhaseTimer(float deltaTime);

	/**
	 * @brief Returns all moving objects to their starting positions
	 * @return void
	 */
	void returnAllToStart();

};

#endif //SEMESTRAL_WORK_GAME_H

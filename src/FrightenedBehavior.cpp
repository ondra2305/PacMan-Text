/**
 * @file FrightenedBehavior.cpp
 * @brief Class representing a frightened ghost behavior
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "FrightenedBehavior.h"
#include <random>
#include <algorithm>
#include <vector>

Direction FrightenedBehavior::move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) {
	std::vector<Direction> directions = {Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT};

	// Remove the opposite of the last move from the directions list
	if (lastMove == Direction::UP) {
		directions.erase(std::remove(directions.begin(), directions.end(), Direction::DOWN), directions.end());
	} else if (lastMove == Direction::DOWN) {
		directions.erase(std::remove(directions.begin(), directions.end(), Direction::UP), directions.end());
	} else if (lastMove == Direction::LEFT) {
		directions.erase(std::remove(directions.begin(), directions.end(), Direction::RIGHT), directions.end());
	} else if (lastMove == Direction::RIGHT) {
		directions.erase(std::remove(directions.begin(), directions.end(), Direction::LEFT), directions.end());
	}

	// Randomly select a direction
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, (int)directions.size() - 1);

	Direction chosenDirection = directions[distr(gen)];
	lastMove = chosenDirection;

	return chosenDirection;
}

void FrightenedBehavior::handleBehavior(MovingObject &enemy, GamePhase &phase) {
	enemy.setInvincible(false);
	enemy.setColor(COLOR_WHITE_CODE);
	enemy.setSpeed(0.5);
}

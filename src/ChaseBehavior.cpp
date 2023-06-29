/**
 * @file ChaseBehavior.cpp
 * @brief Class for handling the chase behavior of ghosts
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "ChaseBehavior.h"
#include <cmath>
#include <limits>
#include <algorithm>

Direction ChaseBehavior::move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) {
	int playerX = player->getX();
	int playerY = player->getY();

	int ghostX = enemy.getX();
	int ghostY = enemy.getY();

	Direction optimalDirection = Direction::NONE;
	int shortestDistance = std::numeric_limits<int>::max();

	// Ordered directions based on priority: UP > LEFT > DOWN > RIGHT.
	std::vector<Direction> directions = {Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT};
	for (const auto &direction: directions) {
		// Skip the direction if it is the opposite of the previous direction
		if ((direction == Direction::UP && enemy.getPrevDirection() == Direction::DOWN) ||
			(direction == Direction::DOWN && enemy.getPrevDirection() == Direction::UP) ||
			(direction == Direction::LEFT && enemy.getPrevDirection() == Direction::RIGHT)) {
			continue;
		}

		int newX = ghostX, newY = ghostY;
		switch (direction) {
			case Direction::UP:
				newY--;
				break;
			case Direction::DOWN:
				newY++;
				break;
			case Direction::LEFT:
				newX--;
				break;
			case Direction::RIGHT:
				newX++;
				break;
			default:
				break;
		}

		int distance = std::abs(playerX - newX) + std::abs(playerY - newY); // Manhattan distance

		// Only update if the new location is not a wall and the distance is less or equal to the shortest so far
		if (!map.isWallAt(newX, newY) && distance <= shortestDistance) {
			shortestDistance = distance;
			optimalDirection = direction;
		}
	}

	prevDirection = optimalDirection;
	return optimalDirection;
}

void ChaseBehavior::handleBehavior(MovingObject &enemy, GamePhase &phase) {
	enemy.setInvincible(true);
	enemy.resetColor();
	enemy.setSpeed(1);
}

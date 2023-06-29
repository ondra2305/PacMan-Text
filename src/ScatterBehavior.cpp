/**
 * @file ScatterBehavior.cpp
 * @brief Class for the scatter behavior of the ghosts
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include <algorithm>
#include <climits>
#include <random>
#include "ScatterBehavior.h"

Direction ScatterBehavior::move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) {
	int ghostX = enemy.getX();
	int ghostY = enemy.getY();

	Direction optimalDirection = Direction::NONE;
	int shortestDistance = INT_MAX;

	// Define corners
	std::vector<std::pair<int, int>> corners = {{0,                       0}, // top-left
												{0,                       map.getSize().second - 1}, // top-right
												{map.getSize().first - 1, 0}, // bottom-left
												{map.getSize().first - 1, map.getSize().second - 1} // bottom-right
	};

	// Ordered directions based on priority: UP > LEFT > DOWN > RIGHT.
	std::vector<Direction> directions = {Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT};
	for (const auto &direction: directions) {
		// Skip the direction if it is the opposite of the previous direction
		if ((direction == Direction::UP && enemy.getPrevDirection() == Direction::DOWN) ||
			(direction == Direction::DOWN && enemy.getPrevDirection() == Direction::UP) ||
			(direction == Direction::LEFT && enemy.getPrevDirection() == Direction::RIGHT) ||
			(direction == Direction::RIGHT && enemy.getPrevDirection() == Direction::LEFT)) {
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

		// Choose a corner randomly
		std::shuffle(corners.begin(), corners.end(), std::mt19937(std::random_device()()));
		std::pair<int, int> corner = corners[0];

		int distance = std::abs(corner.first - newX) + std::abs(corner.second - newY);

		if (!map.isWallAt(newX, newY) && distance < shortestDistance) {
			shortestDistance = distance;
			optimalDirection = direction;
		}
	}

	prevDirection = optimalDirection;
	return optimalDirection;
}

void ScatterBehavior::handleBehavior(MovingObject &enemy, GamePhase &phase) {
	enemy.setInvincible(true);
	enemy.resetColor();
	enemy.setSpeed(1);
}

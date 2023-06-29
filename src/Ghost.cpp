/**
 * @file Ghost.cpp
 * @brief Represents a ghost enemy
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Ghost.h"
#include "Map.h"
#include <algorithm>
#include <iostream>
#include <cfloat>
#include "Helpers.h"
#include "ScatterBehavior.h"
#include "ChaseBehavior.h"
#include "FrightenedBehavior.h"

Ghost::Ghost(int x, int y, float speed, Direction direction, std::unique_ptr<EnemyBehavior> behavior, ColorCode color,
			 float delayLen, char symbol) : MovingObject(x, y, symbol, speed, direction, color) {
	this->behavior = std::move(behavior);
	this->invincible = true;
	this->delayLen = delayLen;
	this->delayed = true;
	this->delayTimer = 0;
}

void Ghost::setBehavior(std::unique_ptr<EnemyBehavior> new_behavior) {
	this->behavior = std::move(new_behavior);
}

bool Ghost::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	if (!other->isHostile() && !other->isInvincible()) {
		other->takeLive();
		other->returnToStart();
		phase.setPhase(GamePhaseState::PlayerDied);
		if (other->getLives() == 0) {
			phase.setPhase(GamePhaseState::PlayerDiedEnd);
		}
	} else if (!other->isHostile() && other->isInvincible()) {
		other->addScore(200 * phase.getLevel());
		this->returnToStart();
		return true;
	}
	return false;
}

void Ghost::move(Direction direction) {
	this->direction = direction;
}

Direction Ghost::getDirectionTowards(int targetX, int targetY) const {
	int dx = targetX - this->x;
	int dy = targetY - this->y;

	if (std::abs(dx) > std::abs(dy)) {
		return dx > 0 ? Direction::RIGHT : Direction::LEFT;
	} else {
		return dy > 0 ? Direction::DOWN : Direction::UP;
	}
}

bool Ghost::tryMove(Direction direction, Map &map) {
	if (!behavior) {
		throw std::runtime_error("Behavior is not set");
	}
	auto [newX, newY] = calculateNewCoordinates(this->x, this->y, direction, 1);
	if ((int) map.getSize().first < newX || (int) map.getSize().second < newY || newX < -1 || newY < -1) {
		throw std::runtime_error("ERROR! Ghost coordinates out of bounds!");
	}
	if (!map.isWallAt(newX, newY) || (newX == (int) map.getNearestGhostCageExit(x, y).first &&
									  newY == (int) map.getNearestGhostCageExit(x, y).second)) {
		move(direction);
		behavior->setPrevDirection(direction);
		return true;
	}
	return false;
}

bool Ghost::getNextMove(std::shared_ptr<MovingObject> &player, Map &map) {
	if (!player) {
		throw std::runtime_error("Player is not set, cannot get next move");
	}
	if (!behavior) {
		throw std::runtime_error("Behavior is not set, cannot get next move");
	}

	std::vector<Direction> directions = {Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT};

	// Get the current direction and its opposite
	Direction currentDirection = this->getDirection();
	Direction opposite = oppositeDirection(currentDirection);

	// Exclude the opposite direction
	directions.erase(std::remove(directions.begin(), directions.end(), opposite), directions.end());
	if (map.isInGhostCage(this->x, this->y) && delayed) {
		if (tryMove(currentDirection, map)) {
			// Move in the current direction
			return true;
		} else {
			// Hit a wall, turn around
			if (currentDirection == Direction::UP) {
				currentDirection = Direction::DOWN;
			} else {
				currentDirection = Direction::UP;
			}

			if (tryMove(currentDirection, map)) {
				return true;
			} else {
				return false;
			}
		}
	} else if (map.isInGhostCage(this->x, this->y)) {
		// If ghost is in the cage, move towards the cage exit
		std::pair<int, int> exit = map.getNearestGhostCageExit(this->x, this->y);
		Direction exitDirection = getDirectionTowards(exit.first, exit.second);
		if (tryMove(exitDirection, map)) {
			return true;
		} else if (tryMove(currentDirection, map)) {
			return true;
		} else {
			for (Direction d: directions) {
				if (tryMove(d, map)) return true;
			}
		}
	} else if (map.isCrossroadAt(this->x, this->y)) {
		// At a crossroad, use the ghost behavior
		Direction wantedDirection = behavior->move(*this, player, map);
		if (wantedDirection == Direction::NONE) {
			throw std::runtime_error("Behavior returned NONE direction");
		}
		if (tryMove(wantedDirection, map)) return true;

		// If the wanted direction leads to a wall, choose the first from preferred list
		for (Direction d: directions) {
			if (tryMove(d, map)) return true;
		}
	} else {
		// Not at crossroad, continue in the current direction
		if (tryMove(currentDirection, map)) return true;

		// If the wanted direction leads to a wall, choose the first preferred list
		for (Direction d: directions) {
			if (tryMove(d, map)) return true;
		}
	}
	return false;
}

bool Ghost::isInvincible() const {
	return invincible;
}

void Ghost::update(float deltaTime, GamePhase &phase) {
	if (delayed && phase.getPhase() != GamePhaseState::NotStarted) {
		delayTimer += deltaTime;
		if (delayTimer >= delayLen) {
			delayTimer = 0.0f;
			delayed = false;
		}
	}

	updateTime += deltaTime;
	const auto currentPhase = phase.getPhase();

	if (currentPhase == GamePhaseState::NotStarted) {
		setBehavior(std::make_unique<ScatterBehavior>());
		behavior->handleBehavior(*this, phase);
	} else if (currentPhase == GamePhaseState::GhostHunting) {
		if (phase.getPrevPhase() != GamePhaseState::GhostHunting) {
			behavior->handleBehavior(*this, phase);
			setBehavior(std::make_unique<FrightenedBehavior>());
		}
	} else if (currentPhase == GamePhaseState::Chase) {
		if (phase.getPrevPhase() != GamePhaseState::Chase) {
			behavior->handleBehavior(*this, phase);
			setBehavior(std::make_unique<ChaseBehavior>());
		}
	} else if (currentPhase == GamePhaseState::Scatter) {
		if (phase.getPrevPhase() != GamePhaseState::Scatter) {
			behavior->handleBehavior(*this, phase);
			setBehavior(std::make_unique<ScatterBehavior>());
		}
	}

	if ((updateTime - (1.0f / (baseSpeed * speedMultiplier))) >= -FLT_EPSILON) {
		updateTime = 0.0f;

		switch (direction) {
			case Direction::UP:
				y--;
				break;
			case Direction::DOWN:
				y++;
				break;
			case Direction::LEFT:
				x--;
				break;
			case Direction::RIGHT:
				x++;
				break;
			case Direction::NONE:
				break;
		}
	}
}

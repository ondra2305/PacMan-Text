/**
 * @file Player.cpp
 * @brief Represents a player
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Player.h"
#include "Helpers.h"
#include <cfloat>

Player::Player(int x, int y, float speed, Direction direction, char symbol, ColorCode color) : MovingObject(x, y,
																											symbol,
																											speed,
																											direction,
																											color) {
	this->score = 0;
	this->nickname = "Player";
	this->invincible = false;
	this->startDirection = Direction::LEFT;
	this->delayLen = 0.0f;
}

bool Player::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	if (other->isHostile()) {
		if (!other->isInvincible() && this->isInvincible()) {
			other->takeLive();
			other->returnToStart();
		} else if (other->isInvincible() && !this->isInvincible()) {
			this->takeLive();
			this->returnToStart();
		}
	}
	return false;
}

void Player::move(Direction direction) {
	this->prevDirection = this->direction;
	this->direction = direction;
}

bool Player::isInvincible() const {
	return invincible;
}

bool Player::getNextMove(std::shared_ptr<MovingObject> &player, Map &map) {
	// Player tries to move in a new direction first
	auto [newX, newY] = calculateNewCoordinates(this->x, this->y, player->getDirection(), 1);
	if ((int) map.getSize().first >= newX && (int) map.getSize().second >= newY && newX > -1 && newY > -1 && !map.isWallAt(newX, newY)) {
		direction = player->getDirection();
		move(direction);
		return true;
	} else {
		// If new direction is blocked, player tries the previous direction
		auto [prevX, prevY] = calculateNewCoordinates(this->x, this->y, player->getPrevDirection(), 1);
		if ((int) map.getSize().first >= prevX && (int) map.getSize().second >= prevY && prevX > -1 && prevY > -1 && !map.isWallAt(prevX, prevY)) {
			direction = player->getPrevDirection();
			move(direction);
			return true;
		}
	}

	// If both are blocked
	return false;
}

void Player::update(float deltaTime, GamePhase &phase) {
	if (delayed) {
		delayTimer += deltaTime;
		if (delayTimer >= delayLen) {
			delayed = false;
		}
	}

	if (!delayed && phase.getPrevPhase() == GamePhaseState::NotStarted) {
		setSpeed(1);
		phase.setPhase(GamePhaseState::Scatter);
	} else if (phase.getPhase() == GamePhaseState::GhostHunting &&
			   phase.getPrevPhase() != GamePhaseState::GhostHunting) {
		setInvincible(true);
		setSpeed(1.5);
	} else if (phase.getPhase() == GamePhaseState::Chase && phase.getPrevPhase() != GamePhaseState::Chase) {
		setInvincible(false);
		setSpeed(1);
	} else if (phase.getPhase() == GamePhaseState::Scatter && phase.getPrevPhase() != GamePhaseState::Scatter) {
		setInvincible(false);
		setSpeed(1);
	}

	updateTime += deltaTime;

	const float interval = 1.0f / (baseSpeed * speedMultiplier);

	if ((updateTime - interval) >= -FLT_EPSILON) {
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


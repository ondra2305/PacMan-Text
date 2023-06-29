/**
 * @file MovingObject.cpp
 * @brief Base class for all moving objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "MovingObject.h"
#include <iostream>

MovingObject::MovingObject(int x, int y, char symbol, float speed, Direction direction) : GameObject(x, y, symbol) {
	this->speedMultiplier = 1;
	this->direction = direction;
	this->startDirection = direction;
	this->prevDirection = Direction::NONE;
	this->startingX = x;
	this->startingY = y;
	this->updateTime = 0.0f;
	this->baseSpeed = 6.0f * speed;
	this->defSpeed = baseSpeed;
	this->delayed = true;
	this->score = 0;
	this->invincible = false;
	this->lives = 0;
	this->delayTimer = 0.0f;
	this->delayLen = 0.0f;
}

MovingObject::MovingObject(int x, int y, char symbol, float speed, Direction direction, ColorCode color) : GameObject(x,
																													  y,
																													  symbol,
																													  color) {
	this->speedMultiplier = 1;
	this->direction = direction;
	this->startDirection = direction;
	this->prevDirection = Direction::NONE;
	this->startingX = x;
	this->startingY = y;
	this->updateTime = 0.0f;
	this->baseSpeed = 6.0f * speed;
	this->defSpeed = baseSpeed;
	this->delayed = true;
	this->score = 0;
	this->invincible = false;
	this->lives = 0;
	this->delayTimer = 0.0f;
	this->delayLen = 0.0f;
}

Direction MovingObject::getDirection() const {
	return direction;
}

void MovingObject::setSpeed(float newSpeedMultiplier) {
	speedMultiplier = newSpeedMultiplier;
}

void MovingObject::setDirection(Direction newDirection) {
	prevDirection = direction;
	direction = newDirection;
}

void MovingObject::setScore(unsigned int newScore) {
	score = newScore;
}

void MovingObject::addScore(unsigned int amount) {
	this->score += amount;
}

unsigned MovingObject::getScore() const {
	return score;
}

unsigned short MovingObject::getLives() const {
	return lives;
}

void MovingObject::setLives(unsigned short livesAmount) {
	lives = livesAmount;
}

void MovingObject::takeLive() {
	this->lives--;
}

void MovingObject::returnToStart() {
	x = startingX;
	y = startingY;
	direction = startDirection;
	delayed = true;
	baseSpeed = defSpeed;
	speedMultiplier = 0;
}

void MovingObject::setInvincible(bool invincibility) {
	this->invincible = invincibility;
}

void MovingObject::setNickname(const std::string &newNickname) {
	nickname = newNickname;
}

const std::string &MovingObject::getNickname() const {
	return nickname;
}

float MovingObject::getBaseSpeed() const {
	return baseSpeed;
}

Direction MovingObject::getPrevDirection() const {
	return prevDirection;
}

void MovingObject::resetDirection() {
	direction = Direction::NONE;
	prevDirection = Direction::NONE;
}

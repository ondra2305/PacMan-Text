/**
 * @file GamePhase.cpp
 * @brief Handles the game phase and level
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "GamePhase.h"

GamePhase::GamePhase() {
	phase = GamePhaseState::NotStarted;
	prevPhase = GamePhaseState::NotStarted;
	phaseTimer = 0;
	remainingItems = 0;
	level = 1;
}

GamePhaseState GamePhase::getPhase() const {
	return phase;
}

void GamePhase::setPhase(GamePhaseState newPhase) {
	setPhaseTime(0.0f);
	this->prevPhase = this->phase;
	this->phase = newPhase;
}

float GamePhase::getPhaseTime() const {
	return phaseTimer;
}

void GamePhase::setPhaseTime(float time) {
	phaseTimer = time;
}

void GamePhase::addPhaseTime(float time) {
	phaseTimer += time;
}

unsigned GamePhase::getRemainingItems() const {
	return remainingItems;
}

void GamePhase::addItem() {
	remainingItems++;
}

void GamePhase::removeItem() {
	remainingItems--;
	if (remainingItems == 0) {
		setPhase(GamePhaseState::LevelComplete);
	}
}

GamePhaseState GamePhase::getPrevPhase() const {
	return prevPhase;
}

unsigned GamePhase::getLevel() const {
	return level;
}

void GamePhase::nextLevel() {
	level++;
}

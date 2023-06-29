/**
 * @file Cherry.cpp
 * @brief Class representing a cherry collectible
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Cherry.h"

bool Cherry::isAccessible() {
	return true;
}

bool Cherry::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	if (!other->isHostile() && !pickedUp) {
		other->addScore(100 * phase.getLevel());
		phase.setPhase(GamePhaseState::GhostHunting);
		pickedUp = true;
		return true;
	}
	return false;
}

bool Cherry::isAccessibleFor(std::shared_ptr<MovingObject> &who) {
	return isAccessible();
}

bool Cherry::isPickupable() {
	return true;
}

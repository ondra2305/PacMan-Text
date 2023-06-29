/**
 * @file Coin.cpp
 * @brief Class representing a coin collectible
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Coin.h"

bool Coin::isAccessible() {
	return true;
}

bool Coin::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	if (!other->isHostile() && !pickedUp) {
		other->addScore(10 * phase.getLevel());
		pickedUp = true;
		return true;
	}
	return false;
}

bool Coin::isAccessibleFor(std::shared_ptr<MovingObject> &who) {
	return isAccessible();
}

bool Coin::isPickupable() {
	return true;
}

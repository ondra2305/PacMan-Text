/**
 * @file Teleport.cpp
 * @brief Represents a teleport object
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Teleport.h"

bool Teleport::isAccessible() {
	return true;
}

bool Teleport::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	int mapWidth = 28;
	int mapHeight = 31;

	if (x == 0) {
		other->setPos(mapWidth - 2, y);
		return false;
	} else if (x == mapWidth - 1) {
		other->setPos(1, y);
		return false;
	} else if (y == 0) {
		other->setPos(x, mapHeight - 2);
		return false;
	} else if (y == mapHeight - 1) {
		other->setPos(x, 1);
		return false;
	}
	return false;
}

bool Teleport::isAccessibleFor(std::shared_ptr<MovingObject> &who) {
	return isAccessible();
}

bool Teleport::isPickupable() {
	return false;
}

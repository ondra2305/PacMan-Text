/**
 * @file Wall.cpp
 * @brief Represents a wall object
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "Wall.h"

bool Wall::isAccessible() {
	return false;
}

bool Wall::interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) {
	return false;
}

Wall::Wall(int x, int y, char symbol, ColorCode color, bool enemyAccessible) : InteractiveObject(x, y, symbol, color),
																			   enemyAccessible(enemyAccessible) {

}

bool Wall::isAccessibleFor(std::shared_ptr<MovingObject> &who) {
	if (who->isHostile() && enemyAccessible) {
		return true;
	}
	return false;
}

bool Wall::isPickupable() {
	return false;
}

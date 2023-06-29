/**
 * @file InteractiveObject.cpp
 * @brief Base class for all interactive objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "InteractiveObject.h"

InteractiveObject::InteractiveObject(int x, int y, char symbol) : GameObject(x, y, symbol) {
	pickedUp = false;
}

InteractiveObject::InteractiveObject(int x, int y, char symbol, ColorCode color) : GameObject(x, y, symbol, color) {
	pickedUp = false;
}

void InteractiveObject::update(float deltaTime, GamePhase &phase) {

}

void InteractiveObject::reset() {
	pickedUp = false;
}

bool InteractiveObject::isPickedUp() const {
	return pickedUp;
}

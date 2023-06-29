/**
 * @file Cherry.h
 * @brief Class representing a cherry collectible
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_CHERRY_H
#define SEMESTRAL_WORK_CHERRY_H

#include "InteractiveObject.h"

class Cherry : public InteractiveObject {
public:
	Cherry(int x, int y) : InteractiveObject(x, y, 'o') {}

	~Cherry() = default;

	bool isAccessible() override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	bool isAccessibleFor(std::shared_ptr<MovingObject> &who) override;

	bool isPickupable() override;
};

#endif //SEMESTRAL_WORK_CHERRY_H

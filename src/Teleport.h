/**
 * @file Teleport.h
 * @brief Represents a teleport object
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_TELEPORT_H
#define SEMESTRAL_WORK_TELEPORT_H

#include "InteractiveObject.h"

class Teleport : public InteractiveObject {
public:
	Teleport(int x, int y) : InteractiveObject(x, y, 'T') {}

	bool isAccessible() override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	bool isAccessibleFor(std::shared_ptr<MovingObject> &who) override;

	bool isPickupable() override;
};

#endif //SEMESTRAL_WORK_TELEPORT_H

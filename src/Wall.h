/**
 * @file Wall.h
 * @brief Represents a wall object
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_WALL_H
#define SEMESTRAL_WORK_WALL_H

#include "InteractiveObject.h"

class Wall : public InteractiveObject {
public:
	Wall(int x, int y, char symbol, ColorCode color, bool enemyAccessible);

	~Wall() = default;

	bool isAccessible() override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	bool isAccessibleFor(std::shared_ptr<MovingObject> &who) override;

	bool isPickupable() override;

private:
	bool enemyAccessible = false;
};

#endif //SEMESTRAL_WORK_WALL_H

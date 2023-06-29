/**
 * @file Coin.h
 * @brief Class representing a coin collectible
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_COIN_H
#define SEMESTRAL_WORK_COIN_H

#include "InteractiveObject.h"

class Coin : public InteractiveObject {
public:
	Coin(int x, int y) : InteractiveObject(x, y, '.') {}

	~Coin() = default;

	bool isAccessible() override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	bool isAccessibleFor(std::shared_ptr<MovingObject> &who) override;

	bool isPickupable() override;
};

#endif //SEMESTRAL_WORK_COIN_H

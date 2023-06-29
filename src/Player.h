/**
 * @file Player.h
 * @brief Represents a player
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_PLAYER_H
#define SEMESTRAL_WORK_PLAYER_H

#include "MovingObject.h"
#include "Map.h"
#include <string>

class Player : public MovingObject {
public:
	Player(int x, int y, float speed, Direction direction, char symbol = '@',
		   ColorCode color = ColorCode::COLOR_YELLOW_CODE);

	~Player() = default;

	[[nodiscard]] bool isHostile() const override { return false; }

	[[nodiscard]] bool isInvincible() const override;

	void update(float deltaTime, GamePhase &phase) override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	void move(Direction direction) override;

	bool getNextMove(std::shared_ptr<MovingObject> &player, Map &map) override;

private:
};

#endif //SEMESTRAL_WORK_PLAYER_H

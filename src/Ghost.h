/**
 * @file Ghost.h
 * @brief Represents a ghost enemy
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_GHOST_H
#define SEMESTRAL_WORK_GHOST_H

#include "MovingObject.h"
#include "EnemyBehavior.h"
#include <memory>
#include "Player.h"
#include "Map.h"

class Ghost : public MovingObject {
public:
	Ghost(int x, int y, float speed, Direction direction, std::unique_ptr<class EnemyBehavior> behavior,
		  ColorCode color, float delayLen, char symbol = '&');

	~Ghost() = default;

	[[nodiscard]] bool isHostile() const override { return true; }

	[[nodiscard]] bool isInvincible() const override;

	void update(float deltaTime, GamePhase &phase) override;

	bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) override;

	void move(Direction direction) override;

	bool getNextMove(std::shared_ptr<MovingObject> &player, Map &map) override;

private:
	std::unique_ptr<EnemyBehavior> behavior;

	void setBehavior(std::unique_ptr<EnemyBehavior> new_behavior);

	[[nodiscard]] Direction getDirectionTowards(int targetX, int targetY) const;

	bool tryMove(Direction direction, Map &map);

};

#endif //SEMESTRAL_WORK_GHOST_H

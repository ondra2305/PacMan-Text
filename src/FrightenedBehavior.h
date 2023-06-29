/**
 * @file FrightenedBehavior.h
 * @brief Class representing a frightened ghost behavior
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_FRIGHTENEDBEHAVIOR_H
#define SEMESTRAL_WORK_FRIGHTENEDBEHAVIOR_H

#include "EnemyBehavior.h"

struct FrightenedBehavior : public EnemyBehavior {
	FrightenedBehavior() = default;

	virtual ~FrightenedBehavior() = default;

	Direction move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) override;

	void handleBehavior(MovingObject &enemy, GamePhase &phase) override;
};

#endif //SEMESTRAL_WORK_FRIGHTENEDBEHAVIOR_H

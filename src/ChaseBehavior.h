/**
 * @file ChaseBehavior.h
 * @brief Class for handling the chase behavior of ghosts
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_CHASEBEHAVIOR_H
#define SEMESTRAL_WORK_CHASEBEHAVIOR_H

#include "EnemyBehavior.h"

struct ChaseBehavior : public EnemyBehavior {
	ChaseBehavior() = default;

	virtual ~ChaseBehavior() = default;

	Direction move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) override;

	void handleBehavior(MovingObject &enemy, GamePhase &phase) override;
};

#endif //SEMESTRAL_WORK_CHASEBEHAVIOR_H

/**
 * @file ScatterBehavior.h
 * @brief Class for the scatter behavior of the ghosts
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_SCATTERBEHAVIOR_H
#define SEMESTRAL_WORK_SCATTERBEHAVIOR_H

#include "EnemyBehavior.h"

struct ScatterBehavior : public EnemyBehavior {
	ScatterBehavior() = default;

	virtual ~ScatterBehavior() = default;

	Direction move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) override;

	void handleBehavior(MovingObject &enemy, GamePhase &phase) override;
};

#endif //SEMESTRAL_WORK_SCATTERBEHAVIOR_H

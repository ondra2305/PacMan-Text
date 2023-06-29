/**
 * @file EnemyBehavior.h
 * @brief Base class for all enemy behaviors
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_ENEMYBEHAVIOR_H
#define SEMESTRAL_WORK_ENEMYBEHAVIOR_H

#include "Map.h"
#include "Enums.h"
#include <memory>
#include "MovingObject.h"

class EnemyBehavior {
public:
	EnemyBehavior() = default;

	virtual ~EnemyBehavior() = default;

	/**
	 * @brief Gets the ideal direction to move the enemy
	 * @param enemy the enemy to move
	 * @param player the player
	 * @param map the map
	 * @return the direction to move
	 */
	virtual Direction move(MovingObject &enemy, std::shared_ptr<MovingObject> player, Map &map) = 0;

	/**
	 * @brief Sets the previous move of the enemy
	 * @param direction the direction
	 */
	void setPrevDirection(Direction direction);

	/**
	 * @brief Handles the behavior of the enemy when changing the game phase
	 * @param enemy the enemy
	 * @param phase the new game phase
	 */
	virtual void handleBehavior(MovingObject &enemy, GamePhase &phase) = 0;

protected:
	Direction lastMove = Direction::NONE;
	Direction prevDirection = Direction::NONE;
};

#endif //SEMESTRAL_WORK_ENEMYBEHAVIOR_H

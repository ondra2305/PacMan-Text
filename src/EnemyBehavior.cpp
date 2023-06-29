/**
 * @file EnemyBehavior.cpp
 * @brief Base class for all enemy behaviors
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "EnemyBehavior.h"
#include "Enums.h"

void EnemyBehavior::setPrevDirection(Direction direction) {
	prevDirection = direction;
}
/**
 * @file GamePhase.h
 * @brief Handles the game phase and level
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_GAMEPHASE_H
#define SEMESTRAL_WORK_GAMEPHASE_H

#include "Enums.h"

class GamePhase {
public:
	GamePhase();

	~GamePhase() = default;

	/**
	 * @brief Returns the current game phase
	 * @return Current game phase
	 */
	[[nodiscard]] GamePhaseState getPhase() const;

	/**
	 * @brief Sets the current game phase
	 * @param phase New game phase
	 */
	void setPhase(GamePhaseState phase);

	/**
	 * @brief Returns the time of the current phase
	 * @return Time of the current phase
	 */
	[[nodiscard]] float getPhaseTime() const;

	/**
	 * @brief Sets the time of the current phase
	 * @param time Time of the current phase
	 */
	void setPhaseTime(float time);

	/**
	 * @brief Adds time to the current phase
	 * @param time Time to add
	 */
	void addPhaseTime(float time);

	/**
	 * @brief Returns the number of remaining items
	 * @return Number of remaining items
	 */
	[[nodiscard]] unsigned getRemainingItems() const;

	/**
	 * @brief Adds an item to the remaining items
	 */
	void addItem();

	/**
	 * @brief Removes an item from the remaining items
	 */
	void removeItem();

	/**
	 * @brief Returns the previous game phase
	 * @return Previous game phase
	 */
	[[nodiscard]] GamePhaseState getPrevPhase() const;

	/**
	 * @brief Returns the current level
	 * @return Current level
	 */
	[[nodiscard]] unsigned getLevel() const;

	/**
	 * @brief Increases the level
	 */
	void nextLevel();

private:
	float phaseTimer;
	unsigned remainingItems;
	unsigned level;

	GamePhaseState phase;
	GamePhaseState prevPhase;
};

#endif //SEMESTRAL_WORK_GAMEPHASE_H

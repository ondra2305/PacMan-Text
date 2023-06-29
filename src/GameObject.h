/**
 * @file GameObject.h
 * @brief Base class for all game objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_GAMEOBJECT_H
#define SEMESTRAL_WORK_GAMEOBJECT_H

#include "Enums.h"
#include "GamePhase.h"
#include <memory>

class GameObject {
public:
	GameObject(int x, int y, char symbol);

	GameObject(int x, int y, char symbol, ColorCode color);

	virtual ~GameObject() = default;

	/**
	 * @brief Updates the game object
	 * @param deltaTime the time since last update
	 * @param phase the game phase
	 */
	virtual void update(float deltaTime, GamePhase &phase) = 0;

	/**
	 * @brief Gets the x coordinate
	 * @return x coordinate
	 */
	[[nodiscard]] int getX() const;

	/**
	 * @brief Gets the y coordinate
	 * @return y coordinate
	 */
	[[nodiscard]] int getY() const;

	/**
	 * @brief Gets the object character symbol
	 * @return symbol
	 */
	[[nodiscard]] char getSymbol() const;

	/**
	 * @brief Gets the object color
	 * @return color
	 */
	[[nodiscard]] ColorCode getColor() const;

	/**
	 * @brief Sets the object color
	 * @param color the new color
	 */
	void setColor(ColorCode color);

	/**
	 * @brief Sets the object position
	 * @param x the new x coordinate
	 * @param y the new y coordinate
	 */
	void setPos(int x, int y);

	/**
	 * @brief Resets the object color to default
	 */
	void resetColor();

protected:
	char symbol;
	ColorCode color;
	ColorCode defColor;
	int x;
	int y;
};

#endif //SEMESTRAL_WORK_GAMEOBJECT_H

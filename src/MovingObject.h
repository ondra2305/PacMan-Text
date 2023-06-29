/**
 * @file MovingObject.h
 * @brief Base class for all moving objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_MOVINGOBJECT_H
#define SEMESTRAL_WORK_MOVINGOBJECT_H

#include "GameObject.h"
#include "Enums.h"
#include <memory>
#include <optional>
#include "Map.h"
#include "GamePhase.h"

class MovingObject : public GameObject {
public:
	MovingObject(int x, int y, char symbol, float speed, Direction direction);

	MovingObject(int x, int y, char symbol, float speed, Direction direction, ColorCode color);

	~MovingObject() = default;

	/**
	 * @brief Updates the object
	 * @param deltaTime the time since last update
	 * @param phase the game phase
	 */
	void update(float deltaTime, GamePhase &phase) override = 0;

	/**
	 * @brief Interacts with another object
	 * @param other the other object
	 * @param phase the game phase
	 * @return true if the interaction was successful
	 */
	virtual bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) = 0;

	/**
	 * @brief Sets the speed of the object
	 * @param speed the new speed
	 */
	void setSpeed(float speed);

	/**
	 * @brief Sets the direction of the object
	 * @param direction
	 */
	void setDirection(Direction direction);

	/**
	 * @brief Returns the direction of the object
	 * @return the direction
	 */
	[[nodiscard]] Direction getDirection() const;

	/**
	 * @brief Returns the object to its starting position
	 */
	void returnToStart();

	/**
	 * @brief Moves the object in the given direction
	 * @param direction
	 */
	virtual void move(Direction direction) = 0;

	/**
	 * @brief Returns the calculated next move of the object
	 * @param player the player
	 * @param map the map
	 * @return true if the move was successful
	 */
	virtual bool getNextMove(std::shared_ptr<MovingObject> &player, Map &map) = 0;

	/**
	 * @brief Sets the score of the object
	 * @param score
	 */
	void setScore(unsigned score);

	/**
	 * @brief Adds the given score to the object
	 * @param score
	 */
	void addScore(unsigned score);

	/**
	 * @brief Returns the score of the object
	 * @return the score
	 */
	[[nodiscard]] unsigned getScore() const;

	/**
	 * @brief Gets the remaining lives
	 * @return the lives
	 */
	[[nodiscard]] unsigned short getLives() const;

	/**
	 * @brief Sets the remaining lives
	 * @param lives
	 */
	void setLives(unsigned short lives);

	/**
	 * @brief Takes one life from the object
	 */
	void takeLive();

	/**
	 * @brief Sets the invincibility of the object
	 * @param invincible
	 */
	void setInvincible(bool invincible);

	/**
	 * @brief Returns the invincibility of the object
	 * @return true if the object is invincible
	 */
	[[nodiscard]] virtual bool isInvincible() const = 0;

	/**
	 * @brief Returns if the object is hostile
	 * @return true if the object is hostile
	 */
	[[nodiscard]] virtual bool isHostile() const = 0;

	/**
	 * @brief Sets the nickname
	 * @param nickname new nickname of the object
	 */
	void setNickname(const std::string &nickname);

	/**
	 * @brief Returns the nickname
	 * @return the nickname
	 */
	[[nodiscard]] const std::string &getNickname() const;

	/**
	 * @brief Gets the base speed of the object
	 * @return the base speed
	 */
	[[nodiscard]] float getBaseSpeed() const;

	/**
	 * @brief Returns the previous direction of the object
	 * @return the previous direction
	 */
	[[nodiscard]] Direction getPrevDirection() const;

	/**
	 * @brief Resets the direction of the object
	 */
	void resetDirection();

protected:
	float speedMultiplier;
	float defSpeed;
	float baseSpeed;
	float updateTime;

	Direction direction;
	Direction startDirection;
	Direction prevDirection;
	unsigned short lives;
	unsigned score;
	int startingX;
	int startingY;
	bool invincible;
	bool delayed;
	float delayTimer;
	float delayLen;

	std::string nickname;
};

#endif //SEMESTRAL_WORK_MOVINGOBJECT_H

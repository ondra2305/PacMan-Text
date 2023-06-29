/**
 * @file InteractiveObject.h
 * @brief Base class for all interactive objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_INTERACTIVEOBJECT_H
#define SEMESTRAL_WORK_INTERACTIVEOBJECT_H

#include "GameObject.h"
#include "Enums.h"
#include <memory>
#include "MovingObject.h"
#include "Player.h"
#include "GamePhase.h"

class InteractiveObject : public GameObject {
public:
	InteractiveObject(int x, int y, char symbol);

	InteractiveObject(int x, int y, char symbol, ColorCode color);

	~InteractiveObject() override = default;

	/**
	 * @brief Updates the game object
	 * @param deltaTime the time since last update
	 * @param phase the game phase
	 */
	void update(float deltaTime, GamePhase &phase) override;

	/**
	 * @brief Interacts with another object
	 * @param other the other object
	 * @param phase the game phase
	 * @return true if interaction was successful
	 */
	virtual bool interactWith(std::shared_ptr<MovingObject> &other, GamePhase &phase) = 0;

	/**
	 * @brief Checks if the object is accessible
	 * @return true if accessible
	 */
	virtual bool isAccessible() = 0;

	/**
	 * @brief Checks if the object is accessible for the given object
	 * @param who the object
	 * @return true if accessible
	 */
	virtual bool isAccessibleFor(std::shared_ptr<MovingObject> &who) = 0;

	/**
	 * @brief Resets the object to its initial state (not picked up)
	 */
	void reset();

	/**
	 * @brief Checks if the object is picked up
	 * @return true if picked up
	 */
	[[nodiscard]] bool isPickedUp() const;

	/**
	 * @brief Checks if the object is pickupable
	 * @return true if pickupable
	 */
	virtual bool isPickupable() = 0;

protected:
	bool pickedUp;
};

#endif //SEMESTRAL_WORK_INTERACTIVEOBJECT_H

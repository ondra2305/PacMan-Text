/**
 * @file GameObject.cpp
 * @brief Base class for all game objects
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include "GameObject.h"

GameObject::GameObject(int newX, int newY, char symbol) {
	this->x = newX;
	this->y = newY;
	this->symbol = symbol;
	this->color = ColorCode::COLOR_WHITE_CODE;
	this->defColor = color;
}

GameObject::GameObject(int newX, int newY, char symbol, ColorCode newColor) {
	this->x = newX;
	this->y = newY;
	this->symbol = symbol;
	this->color = newColor;
	this->defColor = newColor;
}

int GameObject::getX() const {
	return x;
}

int GameObject::getY() const {
	return y;
}

char GameObject::getSymbol() const {
	return symbol;
}

ColorCode GameObject::getColor() const {
	return color;
}

void GameObject::setColor(ColorCode newColor) {
	color = newColor;
}

void GameObject::setPos(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void GameObject::resetColor() {
	this->color = this->defColor;
}
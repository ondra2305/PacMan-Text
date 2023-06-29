#include "Helpers.h"
#include <random>
#include <algorithm>
#include <string>
#include <iostream>

std::string generateRandomNumber(int length) {
	std::string randomNumber;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 9);

	for (int i = 0; i < length; ++i) {
		randomNumber += std::to_string(dist(gen));
	}

	return randomNumber;
}

Direction oppositeDirection(Direction d) {
	switch (d) {
		case Direction::UP:
			return Direction::DOWN;
		case Direction::DOWN:
			return Direction::UP;
		case Direction::LEFT:
			return Direction::RIGHT;
		case Direction::RIGHT:
			return Direction::LEFT;
		default:
			return Direction::NONE;
	}
}

std::pair<int, int> calculateNewCoordinates(int x, int y, Direction direction, int speed) {
	switch (direction) {
		case Direction::UP:
			y -= speed;
			break;
		case Direction::DOWN:
			y += speed;
			break;
		case Direction::LEFT:
			x -= speed;
			break;
		case Direction::RIGHT:
			x += speed;
			break;
		default:
			break;
	}

	return std::make_pair(x, y);
}
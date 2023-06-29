#ifndef SEMESTRAL_WORK_HELPERS_H
#define SEMESTRAL_WORK_HELPERS_H

#include "Enums.h"
#include <string>
#include <vector>

std::string generateRandomNumber(int length);

Direction oppositeDirection(Direction d);

std::pair<int, int> calculateNewCoordinates(int x, int y, Direction direction, int speed);

#endif //SEMESTRAL_WORK_HELPERS_H

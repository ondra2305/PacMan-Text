#ifndef SEMESTRAL_WORK_ENUMS_H
#define SEMESTRAL_WORK_ENUMS_H

#include <utility>

enum Direction {
	UP, DOWN, LEFT, RIGHT, NONE
};

enum GameState {
	InMenu, Playing, Paused, GameOver, Exit
};

enum ColorCode {
	COLOR_YELLOW_CODE = 1, COLOR_RED_CODE, COLOR_MAGENTA_CODE, COLOR_CYAN_CODE, COLOR_WHITE_CODE, COLOR_BLUE_CODE,
};

enum GamePhaseState {
	NotStarted, Chase, Scatter, GhostHunting, PlayerDied, LevelComplete, PlayerDiedEnd
};

#endif //SEMESTRAL_WORK_ENUMS_H

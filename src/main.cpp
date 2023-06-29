/**
 * @file main.cpp
 * @brief Main function, initializes the game manager
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include <iostream>
#include "GameManager.h"

int main() {
	GameManager gameManager;
	try {
		gameManager.start();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
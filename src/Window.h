/**
 * @file Window.h
 * @brief Class for handling ncurses windows
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#ifndef SEMESTRAL_WORK_WINDOW_H
#define SEMESTRAL_WORK_WINDOW_H

#include <ncurses.h>
#include <string>
#include <optional>
#include <vector>
#include "Enums.h"
#include "Helpers.h"

class Window {
public:
	Window() = default;

	~Window() = default;

	/**
	 * @brief Initializes ncurses window
	 */
	void init();

	/**
	 * @brief Exits ncurses window
	 */
	void exit();

	/**
	 * @brief Gets the user choice from the main menu
	 * @return the user choice
	 */
	int getChoice();

	/**
	 * @brief Prints the message to the current window
	 * @param message the message to be printed
	 * @param clear whether to refresh the window before printing
	 * @return void
	 */
	void printMessage(const std::string &message, std::optional<bool> clear = true);

	/**
	 * @brief Prints character at given position, default color
	 * @param x the x coordinate
	 * @param y	the y coordinate
	 * @param c the character to be printed
	 * @param refresh whether to refresh the window after printing
	 */
	void printCharAt(int x, int y, char c, bool refresh);

	/**
	* @brief Prints character at given position with given color
	* @param x the x coordinate
	* @param y	the y coordinate
	* @param c the character to be printed
    * @param color the color of the printed character
	* @param refresh whether to refresh the window after printing
	*/
	void printCharAt(int x, int y, char c, bool refresh, ColorCode color);

	/**
	 * @brief Prints the game UI
	 * @param score player's score
	 * @param lives player's lives
	 * @param level current level
	 * @param remainingItems remaining items to be collected
	 */
	void printUI(unsigned int score, unsigned short lives, unsigned level, unsigned remainingItems);

	/**
	 * @brief Displays the leaderboard
	 * @param topScores the top scores to be displayed and how many
	 * @param gameOver switches between GameOver and Paused leaderboard
	 */
	void displayLeaderboard(const std::vector<std::pair<std::string, unsigned>> &topScores, bool gameOver);

	/**
	 * @brief Clears the current window
	 */
	void clearCurrentWin();

	/**
	 * @brief Refreshes the current window
	 */
	void refreshWin();

	/**
	 * @brief Gets text input from the user
	 * @param prompt
	 * @return std::string input
	 */
	std::string getInput(const std::string &prompt);

private:
	/**
	 * @brief Draws the main menu
	 */
	void drawMenu();

	/**
	 * @brief Creates the main menu window
	 */
	void createMenuWindow();

	/**
	 * @brief Draws the menu options
	 * @param highlight player's highlighted choice
	 * @param choices array of choices
	 * @param n_choices number of choices
	 */
	void drawMenuOptions(int highlight, const char **choices, int n_choices);

	/**
	 * @brief Helper function for printing lives
	 * @param lives
	 */
	void printLives(unsigned short lives);

	int width;
	int height;
	WINDOW *current_win{};
};

#endif //SEMESTRAL_WORK_WINDOW_H

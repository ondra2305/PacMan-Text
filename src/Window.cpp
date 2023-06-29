/**
 * @file Window.cpp
 * @brief Class for handling ncurses windows
 *
 * @author Ondřej Svoboda (svoboo30)
 * @version 1.0
 */

#include <ncurses.h>
#include "Window.h"
#include <cstring>
#include <iostream>
#include <optional>
#include "Config.h"
#include "Enums.h"
#include "Helpers.h"
#include <vector>

void Window::init() {
	initscr();

	// Check for color support
	if (has_colors() == FALSE) {
		endwin();
		throw std::runtime_error("Your terminal does not support color");
	}

	start_color();
	init_pair(COLOR_YELLOW_CODE, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_RED_CODE, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_MAGENTA_CODE, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_CYAN_CODE, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_WHITE_CODE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_BLUE_CODE, COLOR_BLUE, COLOR_BLACK);
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	int windowWidth, windowHeight;
	getmaxyx(stdscr, windowHeight, windowWidth);

	if (windowWidth < 30 || windowHeight < 38) {
		endwin();
		throw std::runtime_error("Terminal window too small! Resize and try again. Minimum size: 38x30, now: " +
								 std::to_string(windowHeight) + "x" + std::to_string(windowWidth) + ".");
	}

	drawMenu();
	refresh();
}

void Window::createMenuWindow() {
	height = 38, width = 30;
	current_win = newwin(height, width, 0, 0);
	refresh();
	clearCurrentWin();
}

void Window::drawMenuOptions(int highlight, const char *choices[], int n_choices) {
	for (int i = 0; i < n_choices; ++i) {
		if (i == highlight)
			wattron(current_win, A_REVERSE);
		mvwprintw(current_win, (getmaxy(current_win) - n_choices) / 2 + i,
				  (getmaxx(current_win) - (int) strlen(choices[i])) / 2, "%s", choices[i]);
		if (i == highlight)
			wattroff(current_win, A_REVERSE);
	}

	wrefresh(current_win);
}

int Window::getChoice() {
	createMenuWindow();

	int highlight = 0;
	const char *choices[] = {"Start New Game", "Load Savefile", "Load Map", "Set save location", "Help", "Exit"};
	int n_choices = sizeof(choices) / sizeof(choices[0]);

	while (true) {
		drawMenuOptions(highlight, choices, n_choices);
		int ch;
		ch = getch();

		switch (ch) {
			case KEY_UP:
				if (highlight == 0)
					highlight = n_choices - 1;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if (highlight == n_choices - 1)
					highlight = 0;
				else
					++highlight;
				break;
			case '\n':
				return highlight;
			case 27: // Esc key
				delwin(current_win);
				current_win = nullptr;
				return 3; // Return "Exit" choice when Esc key is pressed
			default:
				break;
		}
	}
}

std::string Window::getInput(const std::string &prompt) {
	clearCurrentWin();

	int win_height, win_width;
	getmaxyx(current_win, win_height, win_width);

	WINDOW *input_win = newwin(6, win_width - 2, (win_height - 6) / 2, (win_width - (win_width - 2)) / 2);
	box(input_win, 0, 0);
	mvwprintw(input_win, 1, 2, "%s", prompt.c_str());

	// Create a user input sub-window
	WINDOW *sub_input_win = derwin(input_win, 2, win_width - 6, 2, 2);

	wrefresh(input_win);

	char user_input[101];
	echo();

	// Loop until valid input is provided
	while (true) {
		wclrtoeol(input_win);
		box(input_win, 0, 0);
		mvwprintw(input_win, 1, 2, "%s", prompt.c_str());
		wrefresh(input_win);
		wmove(sub_input_win, 0, 0);
		wclrtobot(sub_input_win);

		wgetnstr(sub_input_win, user_input, sizeof(user_input) - 1);

		if (strlen(user_input) == 0) {
			continue;
		}

		bool valid = true;
		for (size_t i = 0; i < strlen(user_input); ++i) {
			char ch = user_input[i];
			if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '-' ||
				  ch == '_' || ch == '/' || ch == '.')) {
				valid = false;
				break;
			}
		}

		if (!valid) {
			wmove(input_win, 5, 2);
			wclrtoeol(input_win);
			box(input_win, 0, 0);
			mvwprintw(input_win, 5, 2, "Invalid characters!");
			wrefresh(input_win);
			getch();
			wrefresh(input_win);

			continue; // Restart the loop to prompt for input again
		}

		break;
	}

	noecho();
	werase(sub_input_win);
	delwin(sub_input_win);
	werase(input_win);
	delwin(input_win);

	std::string userInput(user_input);
	wrefresh(current_win);
	return userInput;
}

void Window::drawMenu() {
	createMenuWindow();
}

void Window::printMessage(const std::string &message, std::optional<bool> clear) {
	if (clear.value_or(false)) clearCurrentWin();

	int win_height, win_width;
	getmaxyx(current_win, win_height, win_width);

	int max_line_length = win_width - 4;

	// Split message into words based on spaces, slashes, and newlines
	std::vector<std::string> words;
	std::string temp;
	for (char c: message) {
		if (c == ' ' || c == '\n') {
			words.push_back(temp);
			temp = "";
			// If newline is encountered, push an empty string as a signal to start a new line
			if (c == '\n') {
				words.emplace_back("");
			}
		} else if (c == '/') {
			temp += c;
			words.push_back(temp);
			temp = "";
		} else {
			temp += c;
		}
	}
	if (!temp.empty()) {
		words.push_back(temp);
	}

	// Calculate how many lines are needed to print the message
	std::vector<std::string> lines;
	std::string line;
	for (const auto &word: words) {
		if (word.empty()) {  // If an empty word is encountered, it's a signal to start a new line
			lines.push_back(line);
			line = "";
		} else if ((int) line.length() + (int) word.length() > max_line_length) {
			lines.push_back(line);
			line = word;
		} else {
			if (!line.empty() && line.back() != '/') {
				line += " ";
			}
			line += word;
		}
	}
	lines.push_back(line);  // push the last line

	int lines_needed = (int) lines.size() + 1;

	WINDOW *msg_win = newwin(lines_needed + 2, win_width - 2, (win_height - lines_needed - 2) / 2,
							 (win_width - (win_width - 2)) / 2);
	box(msg_win, 0, 0);

	for (int i = 0; i < lines_needed - 1; i++) {
		mvwprintw(msg_win, i + 1, 1, "%s", lines[i].c_str());
	}

	wrefresh(msg_win);
	getch();
	delwin(msg_win);
	wrefresh(current_win);
}

void Window::displayLeaderboard(const std::vector<std::pair<std::string, unsigned>> &topScores, bool gameOver) {
	clearCurrentWin();

	int win_height, win_width;
	getmaxyx(current_win, win_height, win_width);

	WINDOW *leaderboard_win = newwin((int) topScores.size() + 4, win_width - 2,
									 (win_height - (int) topScores.size() - 4) / 2, (win_width - (win_width - 2)) / 2);

	box(leaderboard_win, 0, 0);

	if(gameOver)
		mvwprintw(leaderboard_win, 1, (win_width - 10) / 2, "Game Over!");
	else
		mvwprintw(leaderboard_win, 1, (win_width - 10) / 2, "PAUSED");
	mvwprintw(leaderboard_win, 2, 2, "Top Scores:");

	for (size_t i = 0; i < topScores.size(); i++) {
		mvwprintw(leaderboard_win, (int) i + 3, 2, "%s %d", topScores[i].first.c_str(), topScores[i].second);
	}

	wrefresh(leaderboard_win);
	getch();
	delwin(leaderboard_win);
	wrefresh(current_win);
}

void Window::printCharAt(int x, int y, char c, bool shouldRefresh) {
	mvwprintw(current_win, y + 1, x + 1, "%c", c);
	if (shouldRefresh) {
		wrefresh(current_win);
	}
}

void Window::printCharAt(int x, int y, char c, bool shouldRefresh, ColorCode colorPair) {
	wattron(current_win, COLOR_PAIR(colorPair));
	mvwprintw(current_win, y + 1, x + 1, "%c", c);
	wattroff(current_win, COLOR_PAIR(colorPair));

	if (shouldRefresh) {
		wrefresh(current_win);
	}
}

void Window::printLives(unsigned short lives) {
	wmove(current_win, 34, 2);
	wclrtoeol(current_win);
	mvwprintw(current_win, 34, 2, "Lives: ");

	wattron(current_win, COLOR_PAIR(COLOR_YELLOW_CODE));
	if (lives < 10) {
		for (unsigned short i = 0; i < lives; ++i) {
			wprintw(current_win, "@");
		}
	} else {
		wprintw(current_win, "%d", lives);
	}
	wattroff(current_win, COLOR_PAIR(COLOR_YELLOW_CODE));

	wrefresh(current_win);
}


void Window::printUI(unsigned score, unsigned short lives, unsigned level, unsigned remainingItems) {
	mvwprintw(current_win, 1, 2, "Score: %d", score);
	mvwprintw(current_win, 2, 2, "Level: %d", level);
	wmove(current_win, 35, 2);
	wclrtoeol(current_win);
	mvwprintw(current_win, 35, 2, "Remaining: %d", remainingItems);
	wrefresh(current_win);
	printLives(lives);
	refreshWin();
}

void Window::refreshWin() {
	box(current_win, 0, 0);
	wborder(current_win, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(current_win, 0, 0, "Pac-Man: Text");
	wrefresh(current_win);
}

void Window::clearCurrentWin() {
	if (current_win == nullptr)
		throw std::runtime_error("Current window is null");
	werase(current_win);
	box(current_win, 0, 0);
	wborder(current_win, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(current_win, 0, 0, "Pac-Man: Text");
	wrefresh(current_win);
}

void Window::exit() {
	delwin(current_win);
	current_win = nullptr;
	//For memory leak debugging
	//exit_curses(0);
	endwin();
}

# PacMan: Text
## CZ
Tato hra je v podstatě textovým klonem klasického PacMana,
vytvořený s pomocí knihovny ncurses a napsaný v C++17.

Hru jsem vytvořil v rámci mojí semestrální práce v předmětu BI-PA2 
(Programování a algoritmizace 2), LS 2022/23 na FIT ČVUT.

Umožňuje ukládat načítat a ukládat savy, konfiguraci obtížnosti a 
vlastní mapy. [Více ke hře zde](DOCUMENTATION.md).

[Doxygen dokumentace k programu](https://ondra2305.github.io/PacMan-Text/)

## Co je potřeba?
- **ncurses**
   - Ubuntu: sudo apt install libncurses-dev
   - Arch: sudo pacman -S ncurses
- **Linux** *(WSL)*

## Makefile
*make all*
- zkompiluje hru a vytvoří dokumentaci

*make compile*
- pouze zkompiluje hru

*make run*
- zkompiluje a spustí hru

*make doc*
- vytvoří dokumentaci ve složce doc

------------------------------------------------------------------
## EN
This game is basically a text clone of the classic PacMan,
created using the ncurses library and written in C++17.

I created the game as part of my term paper in BI-PA2 
(Programming and Algorithmization 2), LS 2022/23 at FIT CTU.

It allows to save and load saves, configure the difficulty and 
custom maps. [More on the game here](DOCUMENTATION.md).

[Doxygen documentation](https://ondra2305.github.io/PacMan-Text/)

## What is needed?
- **ncurses**
   - Ubuntu: sudo apt install libncurses-dev
   - Arch: sudo pacman -S ncurses
- **Linux** *(WSL)*

## Makefile
*make all*
- compiles the game and creates the documentation

*make compile*
- only compiles the game

*make run*
- compile and run the game

*make doc*
- create documentation in the doc folder

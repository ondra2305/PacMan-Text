# PacMan: Text
Tato hra je v podstatě textovým klonem klasického PacMana,
vytvořený s pomocí knihovny ncurses a napsaný v C++17.

Hru jsem vytvořil v rámci mojí semestrální práce v předmětu BI-PA2 
(Programování a algoritmizace 2), LS 2022/23 na FIT ČVUT.

Umožňuje ukládat načítat a ukládat savy, konfiguraci obtížnosti a 
vlastní mapy. [ Více ke hře zde](DOCUMENTATION.md).

[Doxygen dokumentace k programu](doc/index.html)

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

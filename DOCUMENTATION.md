# PacMan: Text
Ondřej Svoboda

## Popis & requirements
* Jako téma své semestrální práce jsem si vybral Pac-Mana. Můj PacMan je terminálová rekreace originální PacMan hry s pár vylepšeními.
Pro funkčnost je potřeba Linux(nebo alespoň WSL ;-)), knihovna ncurses (libncurses-dev Debian)/ (ncurses Arch).


* Ve složce [examples/maps](examples/maps) jsou 3 základní mapy co jsem vytvořil:
  * předělaná původní mapa PacMana
  * první level Miss PacMan
  * menší easteregg ve formět mapy "Fitman"
* Ve složce [examples/configs](examples/configs) je spousta konfigurací na vyzkoušení.


* Při spuštení je možnost buď začít uplně novou hru s defaultní mapou, nebo nahrát s vlastní mapou a nebo načíst celý save včetně žebříčku nejlepších hráčů.
* Hra se ovládá normálně šipkami/WASD. Esc je pauza a Q ukončíte hru. (viz Help v menu)

## Save
Takhle vypadá struktura savu:
```
/ConfigName
../ConfigName.txt
../Map.txt
../Save
```
- Když načítáte savefile, je potřeba zadat cestu k celé složce (zde "ConfigName")
- Povolené hodnoty v konfiguraci jsou:
    - ghostSpeed - rychlost duchů
    - pacManSpeed - rychlost PacMana
    - ghostHuntingModeDuration - doba lovení duchů
    - amountOfLives - počet životů PacMana
    - (bonusProbability) - zatím není implementováno
- příklad minimálního validního configu, ostatní hodnoty doplní výchozí:
```
configName=Default
map=Maze.txt
saveFile=Save
```

## Mapa
Takhle nějak vypadá příklad validní mapy:
```
############################
#............##............#
#.####.#####.##.#####.####.#
#o#//#.#///#.##.#///#.#//#o#
#.####.#####.##.#####.####.#
#..........................#
#.####.##.########.##.####.#
#.####.##.########.##.####.#
#......##....##....##......#
######.##### ## #####.######
/////#.##### ## #####.#/////
/////#.##          ##.#/////
/////#.## XXX==XXX ##.#/////
######.## X  &   X ##.######
T     .   X      X   .     T
######.## X& & & X ##.######
/////#.## XXXXXXXX ##.#/////
/////#.##          ##.#/////
/////#.## ######## ##.#/////
######.## ######## ##.######
#............##............#
#.####.#####.##.#####.####.#
#.####.#####.##.#####.####.#
#o..##.......@........##..o#
###.##.##.########.##.##.###
###.##.##.########.##.##.###
#......##....##....##......#
#.##########.##.##########.#
#.##########.##.##########.#
#..........................#
############################
```
- Toto jsou všechny povolené znaky, reprezentují:
  - #, X: zdi (X je ohraničení ghost-cage)
  - =: vstup do ghost-cage
  - \@: Pacman
  - &: Duchové
  - ., o: collectibles: coin, cherry (stejné jako v originálu)
  - T: Teleporty
  - /: speciální znak, značí nepřístupná místa (jen design)
- Mapa musí být uzavřená, na okraji mohou být pouze # a T
- Ghost-cage musí mít alespoň jeden vstup
- Mapa musí být průchozí (žádné uzavřené nedostupné části)
- Teleporty musí být vždy v páru
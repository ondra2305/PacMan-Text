LOGIN = svoboo30
CXX = g++
FLAGS = -std=c++17 -O3 -fsanitize=address -Wall -Werror -pedantic
LIBS = -lncurses
#LIBS = -L/lib/ -lncurses_g
#EXTRA_FLAGS = -s -fno-rtti -flto

ZIP = Makefile Doxyfile DOCUMENTATION.md zadani.txt prohlaseni.txt \
  .gitignore $(wildcard examples/*) $(wildcard src/*)

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, build/%.o, ${SOURCES})
DEPS = $(patsubst src/%.cpp, build/%.dep, ${SOURCES})

.PHONY: all compile run valgrind doc clean count zip

all: compile doc

compile: ${LOGIN}

${LOGIN}: ${OBJECTS}
	@mkdir -p build/
	${CXX} ${FLAGS} $^ ${LIBS} -o $@

build/%.o: src/%.cpp
	@mkdir -p build/
	${CXX} ${FLAGS} -c $< -o $@ ${LIBS}

run: compile
	./${LOGIN}

valgrind: compile
	valgrind -s --leak-check=full ./${LOGIN}

doc: doc/index.html

doc/index.html: Doxyfile DOCUMENTATION.md $(wildcard src/*)
	doxygen Doxyfile

count:
	wc -l src/*

clean:
	rm -rf build doc
	rm -f ${LOGIN} ${LOGIN}.zip

zip: ${LOGIN}.zip

${LOGIN}.zip: ${ZIP}
	rm -rf tmp/
	rm -f $@
	mkdir -p tmp/${LOGIN}/
	cp --parents -r $^ tmp/${LOGIN}/
	cd tmp/ && zip -r ../$@ ${LOGIN}/
	rm -rf tmp/

build/%.dep: src/%.cpp src/*
	@mkdir -p build/
	${CXX} -MM -MT $(patsubst src/%.cpp, build/%.o, $<) $< > $@

include ${DEPS}

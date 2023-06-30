FILENAME = PacMan
CXX = g++
FLAGS = -std=c++17 -O3 -fsanitize=undefined -Wall -Werror -pedantic
LIBS = -lncurses

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, build/%.o, ${SOURCES})
DEPS = $(patsubst src/%.cpp, build/%.dep, ${SOURCES})

.PHONY: all compile run doc clean

all: compile doc

compile: ${FILENAME}

${FILENAME}: ${OBJECTS}
	@mkdir -p build/
	${CXX} ${FLAGS} $^ ${LIBS} -o $@

build/%.o: src/%.cpp
	@mkdir -p build/
	${CXX} ${FLAGS} -c $< -o $@ ${LIBS}

run: compile
	./${FILENAME}

doc: doc/index.html

doc/index.html: Doxyfile DOCUMENTATION.md $(wildcard src/*)
	doxygen Doxyfile

clean:
	rm -rf build doc
	rm -f ${FILENAME} ${FILENAME}.zip

build/%.dep: src/%.cpp src/*
	@mkdir -p build/
	${CXX} -MM -MT $(patsubst src/%.cpp, build/%.o, $<) $< > $@

include ${DEPS}

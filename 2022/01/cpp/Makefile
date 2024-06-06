SHELL := /bin/bash
DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
TAG := advent-of-code-$(shell echo $(DIR) | sed 's#/# #g' | xargs -n1 echo | tail -n 3 | xargs echo | sed 's# #-#g')
SRC := solution.cpp
BIN := run

all: format check compile run

format: ${SRC}
	clang-format -style=file -i ${SRC}

check: format ${SRC}
	clang-tidy -config='' ${SRC} -- -std=c++11

compile: check ${SRC}
	clang++ -std=c++11 ${SRC} -o ${BIN}

run: compile
	./${BIN}

clean:
	$(RM) $(BIN)

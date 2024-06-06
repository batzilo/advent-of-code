SHELL := /bin/bash
SRC := solution.cpp
BIN := run

all: format check compile run

format: $(SRC)
	clang-format -style=file -i $(SRC)

check: format $(SRC)
	clang-tidy -config='' $(SRC) -- -std=c++11

compile: check $(SRC)
	clang++ -std=c++11 $(SRC) -o $(BIN)

run: compile
	./$(BIN)

clean:
	$(RM) $(BIN)

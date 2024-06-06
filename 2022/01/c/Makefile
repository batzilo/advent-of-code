# https://matt.sh/howto-c

CC = clang
CFLAGS = -O3 -Wall -Wextra -pedantic -Werror -Wshadow -Wstrict-overflow -fno-strict-aliasing -march=native -std=c99
SRC = solution.c
TGT = run

all: format check compile run

format: $(SRC)
	clang-format -style=file -i $(SRC)

check: format $(SRC)
	clang-tidy -config='' $(SRC) -- $(CFLAGS)

compile: $(SRC)
	$(CC) $(CFLAGS) -o $(TGT) $<

run: compile
	./$(TGT) sample.txt

.PHONY: clean
clean:
	$(RM) *.o $(TGT)

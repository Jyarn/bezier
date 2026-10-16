CC=gcc
LFLAGS=-lasan -lSDL2 -lm
CFLAGS=-fsanitize=address -Wall -Wextra -Wpedantic -g
EXE=main

HEAD=all.h
SRC=main.c render.c glyph.c global.c

.PHONY: clean build

build: $(EXE)

dbg: $(EXE)
	gdb $(EXE) -x .gdbscript

run: $(EXE)
	./$(EXE)

val: $(EXE)
	valgrind --leak-check=full ./$(EXE)

$(EXE): $(addsuffix .o, $(addprefix bin/, $(basename $(SRC))))
	$(CC) $(filter %.o,$^) $(LFLAGS) -o $@

bin/%.o: src/%.c src/all.h
	$(CC) $(CFLAGS) -c $< -o $@

bin/config.o: src/config.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(EXE) bin/*

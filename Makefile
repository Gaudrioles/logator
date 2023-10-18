.PHONY: all clean

# Variables #
CC = gcc
BIN = logator
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

# Flags pour le Compilateur et l'editeur de liens #
ifeq ($(OS),Windows_NT)
CFLAGS = -Wall -Werror -pedantic-errors -O3 -march=znver1
LFLAGS = -lucrt
endif
ifeq ($(shell uname), Linux)
CFLAGS = -Wall -Werror -pedantic-errors -O3 -march=znver1
LFLAGS = 
endif
 
# Construction du programme #

all : $(BIN)

%.o: %.c *.h
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN): $(OBJ)
	$(CC) -s $^ $(LFLAGS) -o $@

# Nettoyage #
clean:
	rm -f $(OBJ) $(BIN)

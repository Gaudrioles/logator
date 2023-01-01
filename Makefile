.PHONY: all clean

# Variables #
CC = gcc
RC = windres
BIN = logator
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
RC_FILE = Resource.rc

# Flags pour le Compilateur et l'editeur de liens #
CFLAGS = -Wall -Werror -pedantic-errors -O3 -march=znver1
LFLAGS =
 
# Construction du programme #

all : $(BIN)

ifeq ($(OS),Windows_NT)
LFLAGS += -lucrt
OBJ += Resource.o

Resource.o:
	$(RC) $(RC_FILE) -o Resource.o
endif

%.o: %.c *.h
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN): $(OBJ)
	$(CC) -s $^ $(LFLAGS) -o $@

# Nettoyage #
clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean

# Variables #
CC = gcc
RC = windres
BIN = logator
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
RC_FILE = Resource.rc

# Flags pour le Compilateur et l'editeur de liens #
CFLAGS =
LFLAGS =
 
# Construction du programme #

all : $(BIN)

ifeq ($(OS),Windows_NT)
CFLAGS += -Wall -O2 -pedantic-errors -mwindows
LFLAGS += -lmingw32
OBJ += Resource.o

Resource.o:
	$(RC) $(RC_FILE) -o Resource.o

%.o: %.c *.h
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN): $(OBJ)
	$(CC) -s $^ $(LFLAGS) -o $@

endif
ifeq ($(shell uname), Linux)
CFLAGS += -Wall -O2 -pedantic-errors

%.o: %.c *.h
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN): $(OBJ)
	$(CC) -s $^ $(LFLAGS) -o $@

endif

# Nettoyage #
clean:
	rm -f $(OBJ) $(BIN)
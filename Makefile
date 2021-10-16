# Variables #
CC = gcc
RC = windres

BIN = logator

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

RC_FILE = Resource.rc

# Flags Compilateur #
 
CFLAGS =
 
# Flags pour l'editeur de liens #
 
LDFLAGS =
 
# Construction du programme #

.PHONY : all
all : $(BIN)

ifeq ($(OS),Windows_NT)
CFLAGS += -Wall -O2 -pedantic-errors -mwindows
LDFLAGS += -lmingw32
OBJ += Resource.o
 
$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)	
%.o: %.c
	$(RC) $(RC_FILE) -o Resource.o
	$(CC) -o $@ -c $< $(CFLAGS)
endif
ifeq ($(shell uname), Linux)
CFLAGS += -Wall -O2 -pedantic-errors

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)	
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
endif

# Nettoyage #

clean:
	rm -f *.o $(BIN)
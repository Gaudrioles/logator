# Variables #
CC = gcc
RC = windres

BIN = logator.exe

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o) Resource.o

RC_FILE = Resource.rc

REP_INSTALL = D:/Bin/logator

# Flags Compilateur #
 
CFLAGS = -Wall -O2 -pedantic-errors
 
# Flags pour l'editeur de liens #
 
LDFLAGS = -lmingw32
 
# Construction du programme #

all: $(BIN)

debug: CFLAGS += -DDEBUG -g
debug: $(BIN)

release: CFLAGS += -mwindows
release: $(BIN)
 
$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)	

%.o: %.c
	$(RC) $(RC_FILE) -o Resource.o
	$(CC) -o $@ -c $< $(CFLAGS)	

install: all
	mkdir -p $(REP_INSTALL)
	mv $(BIN) $(REP_INSTALL)/$(BIN)

uninstall : all
	rm -rf $(REP_INSTALL)

# Nettoyage #

clean:
	rm -f *.o $(BIN)
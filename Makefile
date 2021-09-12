# Variables #
CC = gcc
RC = windres

BIN = logator.exe

SRC_0 = main.c
SRC_1 = fichier.c
SRC_2 = fonction.c
SRC_3 = message.c
	  
OBJ_0 = main.o
OBJ_1 = fichier.o
OBJ_2 = fonction.o
OBJ_3 = message.o

RC_FILE = Resource.rc
RC_OBJ = Resource.o

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
 
$(BIN): $(SRC)
	$(RC) $(RC_FILE) -o $(RC_OBJ)
	$(CC) $(CFLAGS) -c $(SRC_0) -o $(OBJ_0)
	$(CC) $(CFLAGS) -c $(SRC_1) -o $(OBJ_1)
	$(CC) $(CFLAGS) -c $(SRC_2) -o $(OBJ_2)
	$(CC) $(CFLAGS) -c $(SRC_3) -o $(OBJ_3)
	$(CC) $(OBJ_0) $(OBJ_1) $(OBJ_2) $(OBJ_3) $(LDFLAGS) $(RC_OBJ) -o $(BIN)

install: all
	mkdir -p $(REP_INSTALL)
	mv $(BIN) $(REP_INSTALL)/$(BIN)

uninstall : all
	rm -rf $(REP_INSTALL)

# Nettoyage #

clean:
	rm -f *.o $(BIN)
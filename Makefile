CC=g++
SRC=src
BIN=reflect
OBJS=main.o player.o block.o staticBlock.o
FLAGS=-Wall -Werror -c -g
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LIBS)

main.o: $(SRC)/main.cpp
	$(CC) $(FLAGS) $(SRC)/main.cpp

player.o: $(SRC)/player/player.h $(SRC)/player/player.cpp
	$(CC) $(FLAGS) $(SRC)/player/player.cpp

block.o: $(SRC)/block/block.h $(SRC)/block/block.cpp
	$(CC) $(FLAGS) $(SRC)/block/block.cpp

staticBlock.o: $(SRC)/block/staticBlock.h $(SRC)/block/staticBlock.cpp
	$(CC) $(FLAGS) $(SRC)/block/staticBlock.cpp

clean:
	rm -f $(BIN) $(OBJS)

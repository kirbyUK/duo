CC=g++
SRC=src
BIN=reflect
OBJS=main.o player.o block.o staticBlock.o music.o exit.o
FLAGS=-Wall -Werror -c -g
LIBS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

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

music.o: $(SRC)/sound/music.h $(SRC)/sound/music.cpp
	$(CC) $(FLAGS) $(SRC)/sound/music.cpp

exit.o: $(SRC)/level/exit.h $(SRC)/level/exit.cpp
	$(CC) $(FLAGS) $(SRC)/level/exit.cpp

clean:
	rm -f $(BIN) $(OBJS)

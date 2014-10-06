CC=g++
SRC=src
BIN=reflect
OBJS=main.o player.o music.o exit.o level.o block.o staticBlock.o
FLAGS=-Wall -Werror -c -g
LIBS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -g $(OBJS) -o $(BIN) $(LIBS)

main.o: $(SRC)/main.cpp
	$(CC) $(FLAGS) $(SRC)/main.cpp

player.o: $(SRC)/player/player.h $(SRC)/player/player.cpp
	$(CC) $(FLAGS) $(SRC)/player/player.cpp

music.o: $(SRC)/sound/music.h $(SRC)/sound/music.cpp
	$(CC) $(FLAGS) $(SRC)/sound/music.cpp

exit.o: $(SRC)/level/exit.h $(SRC)/level/exit.cpp
	$(CC) $(FLAGS) $(SRC)/level/exit.cpp

level.o: $(SRC)/level/level.h $(SRC)/level/level.cpp
	$(CC) $(FLAGS) $(SRC)/level/level.cpp

block.o: $(SRC)/level/block/block.h $(SRC)/level/block/block.cpp
	$(CC) $(FLAGS) $(SRC)/level/block/block.cpp

staticBlock.o: $(SRC)/level/block/staticBlock.h $(SRC)/level/block/staticBlock.cpp
	$(CC) $(FLAGS) $(SRC)/level/block/staticBlock.cpp

clean:
	rm -f $(BIN) $(OBJS)

CC=g++
SRC=src
BIN=reflect
OBJS=	main.o player.o music.o exit.o level.o button.o block.o staticBlock.o \
		arrow.o

ifdef SystemRoot
    CCFLAGS += -D WIN32
	SFML_PATH=..\lib\SFML-2.1
	JSONCPP_PATH=..\lib\jsoncpp
    FLAGS=-Wall -Werror -c -O3 -I$(SFML_PATH)\include -I$(JSONCPP_PATH)\include
    LIBS=-L$(SFML_PATH)\lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -L$(JSONCPP_PATH)\lib -ljsoncpp
#   DESTDIR="\""C:\Program Files (x86)"\""
#	ASSETS_DIR="$(DESTDIR)\\$(BIN)\\assets"
#	HIGHSCORE_DIR=%appdata%\\$(BIN)
#	HIGHSCORE="$(HIGHSCORE_DIR)\\highscore"
#	ASSETS_DIR="assets"
#	HIGHSCORE="highscore"
#	LOCKFILE="lockfile"
else
	FLAGS=-Wall -Werror -c -g -I/usr/include/jsoncpp/
	LIBS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp
endif

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

button.o: $(SRC)/level/button.h $(SRC)/level/button.cpp
	$(CC) $(FLAGS) $(SRC)/level/button.cpp

block.o: $(SRC)/level/block/block.h $(SRC)/level/block/block.cpp
	$(CC) $(FLAGS) $(SRC)/level/block/block.cpp

staticBlock.o: $(SRC)/level/block/staticBlock.h $(SRC)/level/block/staticBlock.cpp
	$(CC) $(FLAGS) $(SRC)/level/block/staticBlock.cpp

arrow.o: $(SRC)/interface/arrow.h $(SRC)/interface/arrow.cpp
	$(CC) $(FLAGS) $(SRC)/interface/arrow.cpp

levels: level-builder.pl
	./level-builder.pl

clean:
	rm -f $(BIN) $(OBJS)

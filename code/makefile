EXE_DIR = ../exe

TARGET = $(EXE_DIR)/mmain.exe

$(TARGET): main.o Minesweeper.o
	g++ -g -o $(TARGET) main.o Minesweeper.o -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

main.o: main.cpp Minesweeper.h
	g++ -g -c main.cpp -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

Minesweeper.o: Minesweeper.cpp Minesweeper.h
	g++ -g -c Minesweeper.cpp -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

clean:
	rm -f *.o $(TARGET)

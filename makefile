main.exe: main.o
	
main.o: main.cpp
	g++ -g -c main.cpp -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf  -lSDL2_mixer -lSDL2
	g++ -g -o mmain  main.o -Isrc/Include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf  -lSDL2_mixer -lSDL2
clean:
	rm -f *.o main.exe
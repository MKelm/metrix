metrix: main.cpp
	g++ `sdl-config --libs --cflags` -lSDL_gfx -lSDL_mixer -lSDL_ttf -Wall -o metrix main.cpp util.cpp font.cpp audio.cpp field.cpp game.cpp threads.cpp

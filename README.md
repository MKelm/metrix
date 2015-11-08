# Metrix Original Game
Full gameplay with points counter

## Controls
+ Arrow left / NUM4 = Move block to left
+ Arrow right / NUM6 = Move block to right
+ Arror down / NUM2 = Move block to bottom
+ X / NUM7 / NUM1 = Rotate block to left
+ Y / NUM9 / NUM3 = Rotate block to right
+ ESC = Quit Game

## Start parameters
+ --res NUMBER = Resulution, 0 = 800x600 / 1 = 1024/768 * / 2 = 1280x1024
+ --bpp NUMBER = Display bit-depth, 16 / 24 / 32
+ --lvl NUMBER = Difficulty, 0 = easy / 1 = medium ** / 2 = hard
+ --nosound = Deactivate sound support
+ --window = Start in window mode
+ --software = Disable hardware acceleration

(*) Default, the font size is optimized for this resolution
(**) Default, other values make the game field bigger or smaller

## Libary dependencies
Main libraries:
+ SDL (libSDL 1.2)
+ SDL_gfx (libSDL_gfx 1.2.x)
+ SDL_mixer (libSDL_mixer 1.2)
+ SDL_ttf (libSDL_ttf 2.0)

Further libararies:
+ libfreetype-6
+ libogg-0
+ libvorbis-0
+ libvorbisfile-3
+ smpeg
+ zlib1

## Windows Dev-C++
The Dev-C++ project files are included.  
It is recommended to use Dev-C++ Devpaks.

(c) Martin 'yokumo' Kelm, 2008-2015
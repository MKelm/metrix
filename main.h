/**
 * ~metrix - a sdl game with blocks
 * main: Init the global game enviroment
 *
 * Copyright (C) 2008 Martin Kelm
 * https://github.com/MKelm
 *
 * This file is part of the metrix game.
 *
 * This game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL) version 3
 * as published by the Free Software Foundation.
 *
 * This game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _MAIN_H_
#define _MAIN_H_

// Includes
#include "iostream"
using namespace std;

#include "string"
using std::string;

#include "stdio.h"
#include "stdlib.h"

#ifdef WIN32
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

#else
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#endif

#include "util.h"
#include "font.h"
#include "audio.h"
#include "field.h"
#include "game.h"
#include "threads.h"

// Externals
extern SDL_Surface *screen;
extern bool appIsRunning; // Laufzeit-Status

extern int screenWidth;
extern int screenHeight;
extern int screenBPP;

#endif

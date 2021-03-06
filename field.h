/**
 * ~metrix - a sdl game with blocks
 * field: Init a field of blocks and handle them
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

#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

void initField(int difficulty);
int moveAllFieldBlocks();
int removeFieldBlocks();
bool addBlockContainer();

bool rotateBlockContainer(int rotation);
bool moveBlockContainerToSide(int direction);
bool moveBlockContainerDownwards();

void drawFieldBlocks();

extern bool hasBlockContainer;
extern bool hasFallingBlocks;

#endif

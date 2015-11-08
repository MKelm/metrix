/**
 * ~metrix - a sdl game with blocks
 * game: Some basic game functionalities
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

#include "game.h"

int gamePoints = 0;

void addGamePoints(int val) {
  gamePoints = gamePoints + val;
}

/**
 * -----------------------------------------------------------> EXTERNES
 */

/*
 * Zeichnet den Spielhintergrund
 */
void drawGameBackground() {
  // Sklaierung relativ zur Höhe
  int scale = (int)(screenHeight / 16 * 0.125); // Richtwert 1024 / 16 * 0.125 = 8

  for (int y=0; y < screenHeight; y++) {
    // 1. Farbwert für Farbe, 2. für die Skalierung des Verlaufs
    lineRGBA(screen, 0, y, screenWidth-1, y, y/90, y/scale, 0, 255);
  }
}

bool performGameStep() {
  bool moveOn = true;
  // moveAllFieldBlocks siehe field
  addGamePoints(moveAllFieldBlocks()); // Bewege alle Feldblöcke, dabei können Blöcke entfernt werden

  /* Prüfe ob es keinen Block-Container mehr gibt oder keine Blöcke mehr fallen
   * Variablen siehe field */
  if (hasBlockContainer == false || hasFallingBlocks == false) {

    // removeFieldBlock siehe field
    addGamePoints(removeFieldBlocks()); // Entferne Blockhaufen sofern mögl.

    // Versuche einen neuen Block-Container zu setzen, addBlockContainer siehe field
    if (addBlockContainer() == false) {
      // Wenn kein neuer Block-Container gesetzt werden kann ist das Spiel vorbei
      playSound(4); // GameOver-Sound
      moveOn = false;
    }
  }

  return moveOn;
}

/*
 * Ermittelt die aktuelle Verzögerung der Spiel-Schritte.
 * Diese hängt von den Punkten ab, und variiert dadurch die Spielgeschwindigkeit.
 */
int getGameStepDelay(int baseDelay) {
  int stepDelay = baseDelay;

  // die Fallzeit verringert sich je nach Punktestand
  if (gamePoints < 10) {
    stepDelay = baseDelay * 8;
  } else if (gamePoints < 30) {
    stepDelay = baseDelay * 7;
  } else if (gamePoints < 80) {
    stepDelay = baseDelay * 6;
  } else if (gamePoints < 140) {
    stepDelay = baseDelay * 5;
  } else if (gamePoints < 260) {
    stepDelay = baseDelay * 4;
  } else if (gamePoints < 480) {
    stepDelay = baseDelay * 3;
  } else if (gamePoints < 660) {
    stepDelay = baseDelay * 2;
  } else if (gamePoints < 1000) {
    stepDelay = baseDelay * 1;
  }

  return stepDelay;
}


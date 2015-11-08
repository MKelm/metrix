/**
 * ~metrix - a sdl game with blocks
 * font: Init fonts and draw texts
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

// Includes
#ifdef WIN32
  #include "SDL/SDL_ttf.h"
#else
  #include "SDL_ttf.h"
#endif

#include "font.h"

// Definitionen
const int BASE_FONT_SIZE = 36; // Basis-Schriftgröße (ggf. je nach Aufl. variieren)

// Variablen
TTF_Font* baseFont; // Schrift
TTF_Font* creditsFont; // Schrift

SDL_Color textColor = {0xff, 0xff, 0xff};
SDL_Surface *textSurface; // Schriftsurface

bool hasFont;

/*
 * Zeichnet beliebige Texte
 */
void drawText(SDL_Surface *surface, int x, int y,
              const char* str, TTF_Font* font, SDL_Color color) {

  // Temporäres Rechteck um Texte zu positionieren
  SDL_Rect tmpRect;
  tmpRect.x = x;
  tmpRect.y = y;

  // Setze Text-Surface auf übergebenes Screen-Surface
  textSurface = TTF_RenderText_Blended(font, str, color);
  SDL_BlitSurface(textSurface, NULL, surface, &tmpRect);
  SDL_FreeSurface(textSurface);
}

/**
 * -----------------------------------------------------------> EXTERNES
 */

/*
 * Setzt Textelemente im Spiel
 */
void drawGameText(int points) {
  // Ist die Schrift aktiv?
  if (hasFont == true) {

    drawText(
      screen, 10, (screenHeight-BASE_FONT_SIZE+10),
      "Metrix nightly 2008-08-03", creditsFont, textColor
    );

    drawText(
      screen, (screenWidth-(int)(BASE_FONT_SIZE*6.5)), (screenHeight-BASE_FONT_SIZE+10),
      "visit www.idxsolutions.de", creditsFont, textColor
    );

    char strGamePoints[9];
    utilIntToString(points, strGamePoints);

    string tmpStr = "Points: ";
    tmpStr = tmpStr + const_cast<char*>(strGamePoints);
    drawText(
      screen, 10, 10,
      tmpStr.c_str(), baseFont, textColor
    );
  }
}

/*
 * Zeichnet einen Game over text in der Mitte des Bildschirms.
 */
void drawGameOverText() {
  // Ist die Schrift aktiv?
  if (hasFont == true) {

    int fontSize = 64;
    TTF_Font* font = TTF_OpenFont("metrix.ttf", fontSize);
    TTF_Font* font2 = TTF_OpenFont("metrix.ttf", fontSize+2);
    SDL_Color color = {0xff, 0x22, 0x00};
    SDL_Color color2 = {0x00, 0x00, 0x00};
    const char* text = "GAME OVER";

    drawText(
      screen, (int)(screenWidth/2-(9*(fontSize/2+11)/2)), (int)(screenHeight/2-22),
      text, font2, color2
    );

    drawText(
      screen, (int)(screenWidth/2-(9*(fontSize/2+11)/2)), (int)(screenHeight/2-26),
      text, font2, color2
    );

    drawText(
      screen, (int)(screenWidth/2-(9*(fontSize/2+10)/2)), (int)(screenHeight/2-24),
      text, font, color
    );
  }
}

/*
 * Initialisiert die Schriftart zur Ausgabe von Texten
 */
void initFont() {
  hasFont = false;

  if (TTF_Init() == -1) {
    printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());

  } else {
    baseFont = TTF_OpenFont("metrix.ttf", BASE_FONT_SIZE);
    creditsFont = TTF_OpenFont("metrix.ttf", (int)(BASE_FONT_SIZE/2+2));

    if (baseFont == NULL || creditsFont == NULL) {
      printf("Unable to load font: %s %s \n", "metrix.ttf", TTF_GetError());

    } else {
      hasFont = true;
    }
  }
}

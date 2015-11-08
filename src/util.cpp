/**
 * ~metrix - a sdl game with blocks
 * util: Some useful utilities
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

#include "util.h"

/**
 * -----------------------------------------------------------> EXTERNES
 */

/*
 * Hilfsfunktion für intToSring
 */
void utilReverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/*
 * Wandelt Integer-Zahlen in Strings um, buffer muss char[length] sein
 */
void utilIntToString(int n, char s[]) {
  int i, sign;

  if ((sign = n) < 0) { /* record sign */
    n = -n;          /* make n positive */
  }
  i = 0;
  do {       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';   /* get next digit */
  } while ((n /= 10) > 0);     /* delete it */

  if (sign < 0) {
    s[i++] = '-';
  }
  s[i] = '\0';
  utilReverse(s);
}

/*
 * Zufallsgenerator initialisieren
 */
void utilInitRandomizer() {
  srand((unsigned) time(NULL));
}

/*
 * Ermittelt eine Zufallszahl (mit min max Wert),
 * vorher bitte Zufallsgenerator initialisieren
 */
int utilRandomValue(int min, int max) {
  return min + (rand() % (max - min + 1)); // zufälliger Wert zw. a und b
}

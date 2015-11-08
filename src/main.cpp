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

#include "main.h"

// Konstanten
const char* APP_NAME     = "Metrix";
const char* APP_VERSION  = "beta 0.1";

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP    = 32;

// Variablen
SDL_Surface *screen;
bool appIsRunning; // Laufzeit-Status

int screenWidth;
int screenHeight;
int screenBPP;


/*
 * Hauptfunktion, initialisiert SDL Bildschirm und nimmt Parameter entgegen
 */
int main(int argc, char *argv[]) {

  // Setze Eigenschaften zum Initialisieren der Applikation
  bool useAudio = true;
  bool useFullscreen = true;
  bool useHWAcceleration = true;
  int num;
  int difficulty = -1;

  appIsRunning = true;
  screenWidth = SCREEN_WIDTH;
  screenHeight = SCREEN_HEIGHT;
  screenBPP = SCREEN_BPP;

  // Ändere Eigenschaften, sofern der Applikation Argumente übergeben worden
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "--res") && i < argc-1) {
      // Ändere Auflösung
      num = atoi(argv[i+1]);
      switch (num) {
      case 0:
        screenWidth  = 800;
        screenHeight = 600;
        break;
      case 1:
        screenWidth  = 1024;
        screenHeight = 768;
        break;
      case 2:
        screenWidth  = 1280;
        screenHeight = 1024;
        break;
      }
    } else if (!strcmp(argv[i], "--bpp") && i < argc-1) {
      // Ändere BPP-Wert
      num = atoi(argv[i+1]);
      if (num == 16 || num == 24 || num == 32) {
        screenBPP = num;
      }
    } else if (!strcmp(argv[i], "--lvl") && i < argc-1) {
      // Ändere Schwierigkeit
      num = atoi(argv[i+1]);
      if (num == 0 || num == 1 || num == 2) {
        difficulty = num;
      }
    } else  if (!strcmp(argv[i], "--nosound")) {
      // Deaktiviere Audio-Unterstützung
      useAudio = false;
    } else  if (!strcmp(argv[i], "--window")) {
      // Fenstermodus
      useFullscreen = false;
    } else  if (!strcmp(argv[i], "--software")) {
      // Softwaremodus
      useHWAcceleration = false;
    }
  }

  /*
   * Initialisiert SDL,
   * folgende Komponenten sind verfügbar (mit | trennen):
   * SDL_INIT_TIMER
   * SDL_INIT_AUDIO
   * SDL_INIT_VIDEO
   * SDL_INIT_CDROM
   * SDL_INIT_JOYSTICK
   * SDL_INIT_NOPARACHUTE
   * SDL_INIT_EVENTTHREAD
   * SDL_INIT_EVERYTHING
   */
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1) {
    printf("Can't init SDL:  %s\n", SDL_GetError()); // Fehler (SDL)
    exit(1);
  }
  atexit(SDL_Quit); // SDL Ereignis zum Schließen der Applikation

  /* Videomodus setzen,
   * folgende Parameter sind möglich (mit | trennen):
   * SDL_SWSURFACE  - Software Videospeicher
   * SDL_HWSURFACE  - Hardware Videospeicher
   * SDL_ASYNCBLIT  - Asynchrone Aktualisierung (ggf. besser für Multi-Core)
   * SDL_ANYFORMAT  - Wähle irgendein Format, sofern Bit-Tiefe nicht verfügbar
   * SDL_HWPALETTE  - Exklusiver Zugriff auf Farbpalette
   * SDL_DOUBLEBUF  - Hardware Doublebuffering
   * SDL_FULLSCREEN - Vollbildschirm
   * SDL_OPENGL     - OpenGL Render-Kontext, Attribute per SDL_GL_SetAttribute
   * SDL_OPENGLBLIT - OpenGL mit normalem Blitting
   * SDL_RESIZEABLE - Fenstergröße anpassbar
   * SDL_NOFRAME    - Fensterrahmen nicht zeichnen
   */
  Uint32 flags = SDL_DOUBLEBUF|SDL_ASYNCBLIT; // Setze Video-Flags
  if (useHWAcceleration == true) { // Nutze die Hardware?
    flags |= SDL_HWSURFACE;
  } else { // Oder doch nur Software?
    flags |= SDL_SWSURFACE;
  }
  if (useFullscreen == true) { // Setze Vollbildschirm-Modus?
    flags |= SDL_NOFRAME|SDL_FULLSCREEN;
  }
  screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBPP, flags);
  if (screen == NULL) {
    printf("Can't set video mode: %s\n", SDL_GetError()); // Fehler (Video)
    exit(1);

  } else {
    // Setze Fenstertitel mit Applikationsname und -version
    SDL_WM_SetCaption(APP_NAME, 0);

    if (useAudio == true) {
      initAudio(); // Audio-Unterstützung initialisieren
    }
    initFont(); // Text initialisieren
  }
  atexit(SDL_Quit); // SDL Ereignis zum Schließen der Applikation

  // Setzt Blockfarben und Blockfelder
  initField(difficulty); // siehe field

  initThreads(); // Threads initialisieren (startet eigenständige Threads);
  runMainThread(); // Hauptthread ausführen (Schleife)
  destructThreads(); // Threads aufheben

  destructAudio(); // Audio-Unterstützung aufheben

  SDL_Quit();
  return 0;
}

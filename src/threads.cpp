/**
 * ~metrix - a sdl game with blocks
 * threads: Handle and perform different games threads
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
  #include "SDL/SDL_thread.h"
  #include "SDL/SDL_mutex.h"
#else
  #include "SDL_thread.h"
  #include "SDL_mutex.h"
#endif

#include "threads.h"

// Definitionen
const int BASE_DELAY = 100;

// Mutex zum Synchronisieren von Threads
SDL_mutex *lock;

// Eigenständige Threads
SDL_Thread *threadGameStepRun;
SDL_Thread *threadBaseRun;

/*
 * Dieser Thread führt die laufenden Spiel-Schritte aus
 */
int threadGameStep(void *data)  {
  SDL_mutex *threadLock = (SDL_mutex *)data;

  /* TODO:
   * Später muss noch geklärt werden, wann der Thread läuft und wann nicht,
   * z.B. wenn ein Spiel startet oder aber wann das Menü aktiv ist.
   * Zu beachten ist, dass ggf. die Initialisierung des Felds neu erfolgen
   * muss etc.
   */
  do {
    SDL_mutexP(threadLock); // Threadsperre setzen
    appIsRunning = performGameStep();
    SDL_mutexV(threadLock); // Threadsperre aufheben

    SDL_Delay(getGameStepDelay(BASE_DELAY));

  } while (appIsRunning == true);

  // Sofern keine Spielschritte mehr erfolgen, ist das Spiel vorbei
  SDL_mutexP(threadLock); // Threadsperre setzen
  drawGameOverText(); // todo als draw funtion oder so
  SDL_Flip(screen);
  SDL_mutexV(threadLock); // Threadsperre aufheben
  SDL_Delay(BASE_DELAY * 30);

  return 0;
}

/*
 * Der Basis-Thread führt die meisten regelmäßigen Aktionen aus
 */
int threadBase(void *data)  {
  SDL_mutex *threadLock = (SDL_mutex *)data;

  do {
    SDL_mutexP(threadLock); // Threadsperre setzen

    /* TODO:
     * Sofern ein Menü eingebunden wird, sind hier noch Anpassungen notwendig.
     */
    drawGameBackground(); // siehe game
    drawFieldBlocks(); // siehe field
    drawGameText(gamePoints);

    SDL_Flip(screen);
    SDL_mutexV(threadLock); // Threadsperre aufheben

    // dieses kleine Delay senkt die Prozessorlast
    SDL_Delay(BASE_DELAY);
  } while (appIsRunning == true);

  return 0;
}

/*
 * Der Hauptthread wird vom Hauptprogramm aus aufgerufen,
 * und brauch nicht als zusätzlicher Thread initialisiert zu werden.
 */
void threadMain(void *data) {
  SDL_Event event;
  SDL_mutex *threadLock = (SDL_mutex *)data;

  // Verarbeite Events, solang das Programm läuft
  while (appIsRunning == true) {

    SDL_mutexP(threadLock); // Threadsperre setzen

    /* TODO:
     * Die Eventbehandlung muss später noch separiert und ausgelagert werden,
     * so enthält das Spiel selbst andere Events als das Menü.
     */
    // Ermittle Ereignis und beende Laufzeit
    while (SDL_PollEvent(&event)) {

      switch (event.type) {
      case SDL_KEYDOWN:
        // Ereignis: Taste
        switch (event.key.keysym.sym) {
        case SDLK_y:
        case SDLK_KP7:
        case SDLK_KP1:
          // 7 / 1 -> Blockgruppe nach links rotieren
          rotateBlockContainer(-1); // siehe field
          break;
        case SDLK_x:
        case SDLK_KP9:
        case SDLK_KP3:
          // 9 / 3 -> Blockgruppe nach rechts rotieren
          rotateBlockContainer(+1); // siehe field
          break;
        case SDLK_LEFT:
        case SDLK_KP4:
          // 4 -> Nach links verschieben
          moveBlockContainerToSide(-1); // siehe field
          break;
        case SDLK_RIGHT:
        case SDLK_KP6:
          // 6 -> Nach rechts verschieben
          // todo actionMovement
          moveBlockContainerToSide(+1); // siehe field
          break;
        case SDLK_DOWN:
        case SDLK_KP2:
          // 2 -> Nach untern verschieben
          moveBlockContainerDownwards();  // siehe field
          break;
        case SDLK_ESCAPE:
          // Beende Applikation
          appIsRunning = false;
          break;
        default:
          break;
        }
        break;
      case SDL_QUIT:
        // Beende Applikation
        appIsRunning = false;
        break;
      default:
        break;
      }
    }

    SDL_mutexV(threadLock); // Threadsperre aufheben

    // dieses kleine Delay senkt die Prozessorlast
    SDL_Delay(BASE_DELAY);
  }
}

/**
 * -----------------------------------------------------------> EXTERNES
 */

/*
 * Initialisiert den Mutex und die eigenständigen Threads
 */
void initThreads() {
  lock = SDL_CreateMutex();

  threadGameStepRun = SDL_CreateThread(threadGameStep, lock);
  threadBaseRun = SDL_CreateThread(threadBase, lock);
}

/*
 * Externe Wrapper-Funktion zum Starten des Hauptthreads
 */
void runMainThread() {
  threadMain(lock);
}

/*
 * Ein kleiner Destruktor für Threads, der auch auf laufende Threads wartet
 */
void destructThreads() {
  SDL_WaitThread(threadGameStepRun, NULL);
  SDL_WaitThread(threadBaseRun, NULL);

  SDL_DestroyMutex(lock);
}

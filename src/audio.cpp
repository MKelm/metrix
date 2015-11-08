/**
 * ~metrix - a sdl game with blocks
 * audio: Init sound files and play it!
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
  #include "SDL/SDL_mixer.h"
#else
  #include "SDL_mixer.h"
#endif

#include "audio.h"

// Definitionen
const int SOUNDS_AMOUNT = 5;

// Sound-Datentyp und Variable
typedef struct {
  const char* fileName;
  Mix_Chunk *chunk;
} structSound;

structSound sound[SOUNDS_AMOUNT];

bool hasAudio;

/*
 * Sound-Dateien initialisieren
 */
void initSoundFiles() {
  sound[0].fileName = "movement.wav";
  sound[1].fileName = "rotation.wav";
  sound[2].fileName = "new_blocks.wav";
  sound[3].fileName = "remove_blocks.wav";
  sound[4].fileName = "game_over.wav";
}

/*
 * Sounds bzw. die Chunks laden
 */
bool loadSounds() {
  // Je nach OS sind ist das Sound-Verzeichnis anders anzusprechen
  string audioDir;
  #ifdef WIN32
    audioDir = ".\\snd\\";
  #else
    audioDir = "snd/";
  #endif

  // Versuche Sound-Dateien zu laden
  bool success = true;
  string tmpStr;
  for (int i=0; i < SOUNDS_AMOUNT; i++) {
    tmpStr = audioDir + sound[i].fileName;
    sound[i].chunk = Mix_LoadWAV(tmpStr.c_str());
    if (sound[i].chunk == 0) {
      success = false;
    }
  }

  // Gibt zurück, ob alle Sound erfolgreich geladen wurden
  return success;
}

/**
 * -----------------------------------------------------------> EXTERNES
 */

/*
 * Initialisiert die Audio-Unterstützung
 */
void initAudio() {
  hasAudio = false;  // Audio verfügbar?

  // Überprüfe ob die Audio-Komponente verfügbar ist
  if(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) {

    // audio settings
    int audio_rate      = 22050;
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels  = 2;
    int audio_buffers   = 4096;

    // Öffne Audiomixer um Sounds zu laden
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == 0) {

      initSoundFiles();
      // Lade Sounds und überprüfe ob alle geladen wurden
      if (loadSounds() == false) {
        printf("Note: Some sound files in ./snd have not been loaded.\n");

      } else {
        // die Audio-Unterstützung wurde erfolgreich initialisiert
        hasAudio = true;
      }
    }
  }
}

/*
 * Spielt Sounds je nach Index, und prüft zuvor, ob dieser Sound verfügbar ist
 */
void playSound(int idx) {
  // Ist die Audio-Unterstützung aktiv?
  if (hasAudio == true) {
    // Ist der Index gültig?
    if (idx >= 0 && idx < SOUNDS_AMOUNT) {
      // Ist der Sound-Chunk verfügbar?
      if (sound[idx].chunk != 0) {
        // ... dann spiele ihn!
        Mix_PlayChannel(-1, sound[idx].chunk, 0);
      }
    }
  }
}

/*
 * Ein kleiner Destruktor für die Audio-Unterstützung
 */
void destructAudio() {
  if (hasAudio == true) {
    Mix_CloseAudio();
  }
}

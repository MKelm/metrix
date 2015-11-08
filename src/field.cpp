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

#include "field.h"

int FIELD_COLS; // wird in initField gesetzt
int FIELD_ROWS; // wird in initField gesetzt

const int BLOCK_COLORS_AMOUNT = 1+5; // Index 0 ist leer / farblos
const int BLOCK_CONTAINER_SIZE = 3; // Muss ungerade sein, damit die Rotation synchron läuft

int fieldBlocks[12][16]; // Mehrdim. Array für Feldblöcke, hat die max. Größe von 12 / 16
int blockColors[BLOCK_COLORS_AMOUNT][2][3]; // Mehrdimensionales Array mit RGB-Block-Farben

int blockContainer[BLOCK_CONTAINER_SIZE][3]; // Array für einen Block-Container, enthält Block-Feld-Koordinaten usw.

bool hasBlockContainer; // Gibt es derzeit einen intakten / fallenden Block-Container
bool hasFallingBlocks; //Gibt es derzeit fallende Blöcke

/**  ------------------------------------------------------ INITIALISIERUNG **/

/*
 * Initialisiert die Farben der Feldblöcke
 */
void initBlockColors() {

  // 0 = Transparent
  // 1 Rot / BG-Fläche
  blockColors[1][0][0] = 163; // R
  blockColors[1][0][1] = 34; // G
  blockColors[1][0][2] = 34; // B
  // 1 Rot / FG-Fläche
  blockColors[1][1][0] = 209; // R
  blockColors[1][1][1] = 24; // G
  blockColors[1][1][2] = 24; // B

  // 2 Grün / BG-Fläche
  blockColors[2][0][0] = 85; // R
  blockColors[2][0][1] = 157; // G
  blockColors[2][0][2] = 41; // B
  // 2 Grün / FG-Fläche
  blockColors[2][1][0] = 100; // R
  blockColors[2][1][1] = 185; // G
  blockColors[2][1][2] = 49; // B

  // 3 Blau / BG-Fläche
  blockColors[3][0][0] = 41; // R
  blockColors[3][0][1] = 147; // G
  blockColors[3][0][2] = 157; // B
  // 3 Blau / FG-Fläche
  blockColors[3][1][0] = 49; // R
  blockColors[3][1][1] = 174; // G
  blockColors[3][1][2] = 185; // B

  // 4 Gelb / BG-Fläche
  blockColors[4][0][0] = 157; // R
  blockColors[4][0][1] = 145; // G
  blockColors[4][0][2] = 41; // B
  // 4 Gelb / FG-Fläche
  blockColors[4][1][0] = 185; // R
  blockColors[4][1][1] = 171; // G
  blockColors[4][1][2] = 49; // B

  // 5 Magenta / BG-Fläche
  blockColors[5][0][0] = 128; // R
  blockColors[5][0][1] = 41; // G
  blockColors[5][0][2] = 157; // B
  // 5 Magenta / FG-Fläche
  blockColors[5][1][0] = 152; // R
  blockColors[5][1][1] = 49; // G
  blockColors[5][1][2] = 185; // B
}

/*
 * Setzt das Feld, alle Felder sind zu Anfang leer
 * bzw. sind nicht mit einer Blockfarbe belegt.
 */
void initField(int difficulty) {

  // Setze Standard für Schwierigkeitsgrad
  if (!(difficulty >= 0 && difficulty < 3)) {
    difficulty = 1;
  }

  /*
   * Je nach Schwierigkeit differnziert die Feldgröße
   * easy 16 / 12
   * middle 14 / 10
   * hard 12 / 8
   */
  switch (difficulty) {
  case 2: // hard
    FIELD_COLS  = 8;
    FIELD_ROWS = 12;
    break;
  case 1: // normal
    FIELD_COLS  = 10;
    FIELD_ROWS = 14;
    break;
  case 0: // easy
  default:
    FIELD_COLS  = 12;
    FIELD_ROWS = 16;
    break;
  }

  hasBlockContainer = false;
  hasFallingBlocks  = false;

  initBlockColors();

  // Belegt alle Felder mit dem Blockfarben Index für Transparenz
  for (int x=0; x < FIELD_COLS; x++) {
    for (int y=0; y < FIELD_ROWS; y++) {
      fieldBlocks[x][y] = 0;
    }
  }

  // Blockcontainer initialisieren, diesen Container kann der Spieler steuern
  for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
    blockContainer[i][0] = -1; // x
    blockContainer[i][1] = -1; // y
    blockContainer[i][2] = -1; // Hilfsvariable zum verschieben von Blockgruppen
  }
}

/**  ----------------------------------------------------------- HELFERLEIN **/

/*
 * Ermittelt den Index eines Blocks anhand den x, y Koordinaten
 */
int getBlockContainerIndex(int x, int y) {
  for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
    if (blockContainer[i][0] == x && blockContainer[i][1] == y) {
      return i;
    }
  }
  return -1;
}

/*
 * Ermittelt ob eine Blockgruppe waagerecht oder senkrecht liegt.
 */
int getBlockContainerState() {
  int xEqual = 1;
  int yEqual = 1;

  for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
    if (i > 0) {
      if (blockContainer[i-1][0] != blockContainer[i][0] || blockContainer[i][0] == -1) {
        xEqual = 0; // keine gleichen x-Werte
      }
      if (blockContainer[i-1][1] != blockContainer[i][1] || blockContainer[i][1] == -1) {
        yEqual = 0; // keine gleichen y-Werte
      }
    }
  }

  if (xEqual == 1) {
    return 1; // Container ist waagerecht ausgerichtet
  } else if (yEqual == 1) {
    return 2; // Container ist senkrecht ausgerichtet
  }
  return 0; // keine gültige Ausrichtung
}

/**  ------------------------------------------------------------- ZEICHNEN **/

/*
 * Zeichnet Blöcke dynamisch, je nach Fenstergröße und Spalten-/Zeilenanzahl.
 */
void drawFieldBlock(int posX, int posY, int colorKey, int xLeft) {

  // Wandle die Breite / Höhe in Index-Werte um
  int w = screenWidth-1;
  int h = screenHeight-1;

  // Hilfswerte für Blockbreite / -höhe
  int blockWidth        = (int)((w*0.5) / FIELD_COLS); // Hälfte des Bildes durch Spaltenanzahl
  int blockBorderWidth  = (int)(blockWidth / FIELD_COLS); // Blockbreite durch Spaltenanzahl
  int blockHeight       = (int)(h / FIELD_ROWS); // Höhe durch Reihenanzahl
  int blockBorderHeight = (int)(blockHeight / FIELD_ROWS); // Blockhöhe durch Reihenanzahl

  // X Positionen von 0 bis BLOCK_COLUMNS-1
  int moveRight = blockWidth * posX;
  // Y Positionen von 0 bis BLOCK_ROWS-1
  int moveDown = h-(int)(h/FIELD_ROWS)*(FIELD_ROWS-posY); // Wird abgezogen, damit's Unten bündig wird

  // Block Hintergrund
  int sx = moveRight + xLeft; // x-Quelle, Seitenrand ergibt sich aus vorher berechnetem xLeft
  int sy = moveDown  + 0; // y-Quelle, erste Zeile
  int dx = moveRight + xLeft + blockWidth; // x-Ziel, die Breite ist ein 5tel des Blockbereichs
  int dy = moveDown  + blockHeight; // y-Ziel, die Höhe ist ein 7tel
  boxRGBA(screen, sx, sy, dx, dy,
    blockColors[colorKey][0][0],
    blockColors[colorKey][0][1],
    blockColors[colorKey][0][2], 255);

  // Block Vordergrund
  sx = moveRight + xLeft + blockBorderWidth; // x-Quelle,  Seitenrand ergibt sich aus vorher berechnetem xLeft
  sy = moveDown  + blockBorderHeight; // y-Quelle, der Abstand ergibt sich aus dem 7tel Teil
  dx = moveRight + xLeft + blockWidth - blockBorderWidth; // x-Quelle, ergibt sich aus einer verschobenen Blockbreite
  dy = moveDown  + blockHeight- blockBorderHeight; // y-Ziel, Abstand wird von Blockhöhe abgezogen
  boxRGBA(screen, sx, sy, dx, dy,
    blockColors[colorKey][1][0],
    blockColors[colorKey][1][1],
    blockColors[colorKey][1][2], 255);
}

/*
 * Zeichne den Blockbereich dynamisch zur Breite. Da die Blöcke einzeln gezeichnet
 * werden, wurde die Rundungsdifferenz bei der Berechnung der Breite des Bereichs
 * berücksichtigt.
 */
void drawFieldBlocks() {

  // Wandle die Breite / Höhe in Index-Werte um
  int w = screenWidth-1;
  int h = screenHeight-1;

  /* Verlust durch Rundung in der Breite errechnen. Diese ergibt sich aus späterer
   * Blockberechnung. Daraus resultiert ein linker und rechter x-Wert.
   */
  int widthFix = 0;
  for (int i = 0; i < FIELD_COLS; i++) {
    widthFix += (int)((w*0.5)/FIELD_COLS);
  }
  int xLeft = (int)(w-widthFix)/2;
  int xRight = w-xLeft-1; // beim Überstehenden Pixel ist zu prüfen, ob das bei jeder Aufl. auftaucht

  // Bereich für Blöcke
  boxRGBA(screen, xLeft, 0, xRight, h, 240, 240, 240, 80);

  // Setze Blöcke im Blockbereich, je nach Spalte/Zeile mit jew. Farbe
  for (int x=0; x < FIELD_COLS; x++) {
    for (int y=0; y < FIELD_ROWS; y++) {

      if (fieldBlocks[x][y] > 0) {
        // Der linke x-Wert wird mit übergeben, damit alle Blöcke daran ausgerichtet werden
        drawFieldBlock(x, y, fieldBlocks[x][y], xLeft);
      }
    }
  }
}

/**  ------------------------------------------------------------- AKTIONEN **/

/* Setze Block-Container zurück, um später einen neuen zu setzen. */
void resetBlockContainer() {
  for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
    blockContainer[i][0] = -1; // x
    blockContainer[i][1] = -1; // y
    blockContainer[i][2] = -1; // hilfsvariable zum verschieben von blockgruppen
    hasBlockContainer = false;
  }
}

/*
 * Fügt einen neuen Block-Container hinzu.
 */
bool addBlockContainer() {

  utilInitRandomizer();
  int minContainerPos = utilRandomValue(0, FIELD_COLS - BLOCK_CONTAINER_SIZE);
  int maxContainerPos = minContainerPos + BLOCK_CONTAINER_SIZE;

  int i = 0; // Container-Index
  /* Konnte der Container erfolgreich gesetzt werden?
   * Sofern andere Blöcke blockieren wird der Wert false und kann
   * zu einem Spielende führen. */
  bool success = true;

  utilInitRandomizer(); // Zufallsgenerator noch einmal neu setzen
  for (int x = minContainerPos; x < maxContainerPos; x++) {
    // Gehe durch Container Positionen
    if (success == true) {
      // Überprüfe ob der Feldblock schon belegt ist
      if (fieldBlocks[x][0] != 0) {
        success = false;
      } else {
        // ... ansonsten belege den Feldblock neu
        fieldBlocks[x][0] = utilRandomValue(1, BLOCK_COLORS_AMOUNT-1);
        // Referenziere den Feldblock im Block-Container
        blockContainer[i][0] = x;
        blockContainer[i][1] = 0;
        i++; // Nächster bitte ...
      }
    }
  }
  // Sofern kein Fehler unterlaufen ist, hat das Feld einen aktiven Block-Container
  if (success == true) {
    hasBlockContainer = true;
    playSound(2); // Sound beim Setzen eines neuen Block-Containers
  }
  return success;
}

/*
 * Diese Funktion entfernt einen Blockhaufen rekursiv (wie Flood-Fill)
 * Mit dem Parameter "check" werden Feldblöcke nur überprüft,
 * dabei werden die Farbwerte jedoch invertiert, damit die Rekursion diese,
 * in Folge, ignoriert. Die Funktion kann erneut in diesem Modus ausgeführt
 * werden, um die Feldblöcke wieder in den ursprünglichen Zustand zu setzen.
 */
int removeBlockBunch(int x, int y, int color, bool check) {
  int sum = 1;

  // check modus
  if (check == true) {
    // invertiere Blockwert um Rekursionsschleifen zu vermeiden
    fieldBlocks[x][y] = -1 * fieldBlocks[x][y];
  } else {
    fieldBlocks[x][y] = 0;
  }

  // überprüfe / addiere umliegende Feldblöcke, sofern die Farbe übereinstimmt
  if(x > 0 && fieldBlocks[x-1][y] == color) {
    // Hier muss auch überprüft werden, ob sich der linke Block nicht im Fall befindet
    if (!(y+1 < FIELD_ROWS && fieldBlocks[x-1][y+1] == 0)) {
      sum += removeBlockBunch(x-1, y, color, check);
    }
  }
  if(y > 0 && fieldBlocks[x][y-1] == color) {
    sum += removeBlockBunch(x, y-1, color, check);
  }
  if(x+1 < FIELD_COLS && fieldBlocks[x+1][y] == color) {
    // Hier muss auch überprüft werden, ob sich der linke Block nicht im Fall befindet
    if (!(y-1 < FIELD_ROWS && fieldBlocks[x+1][y+1] == 0)) {
      sum += removeBlockBunch(x+1, y, color, check);
    }
  }
  if(y+1 < FIELD_ROWS && fieldBlocks[x][y+1] == color) {
    // Hier muss auch überprüft werden, ob sich der linke Block nicht im Fall befindet
    if (!(y+2 < FIELD_ROWS && fieldBlocks[x][y+2] == 0)) {
      sum += removeBlockBunch(x, y+1, color, check);
    }
  }

  return sum;
}

/*
 * Entfernt Blöcke, sofern möglich, von einer gegebenen Feldkoordinate aus.
 * Die Koordinate muss innerhalb des Felds sein und muss einen Block enthalten.
 * Die Funktion nutzt removeBlockBunch und wird einerseits von removeFieldBlocks
 * genutzt, ansonsten von moveAllFieldBlocks, sofern ein Block dabei seine
 * Endposition erreicht.
 */
int removeBlocksByCoor(int x, int y) {
  int removed = 0;

  // ist die Koordinate innerhalb des spielfelds?
  if (x < FIELD_COLS && y < FIELD_ROWS) {
    int color = fieldBlocks[x][y];
    // kein leerer Feldblock
    if (color != 0) {
      // überprüfe ob der Haufen groß genug ist
      int checked = removeBlockBunch(x, y, color, true);
      if (checked > BLOCK_CONTAINER_SIZE) {
        // entferne den Haufen und zähle den Zähler hoch
        // verwende die invertierte Blockfarbe
        removeBlockBunch(x, y, -1*color, false);
        playSound(3); // Sound beim Entfernen eines Blockhaufens
        removed = removed + checked;
      } else {
        // ansonsten hebe die invertierte Blockfarbe auf
        removeBlockBunch(x, y, -1*color, true);
      }
    }
  }

  return removed;
}

/*
 * Sucht das gesamte Feld nach Blöcken ab und versucht ggf. gleichfarbige
 * Blockhaufen zu entfernen. Diese Methode wird immer dann aufgerufen,
 * wenn eine neue Blockgruppe gesetzt wird.
 */
int removeFieldBlocks() {
  int removed = 0;

  for (int x=0; x < FIELD_COLS; x++) {
    for (int y=0; y < FIELD_ROWS; y++) {
      /* Versuche Feldblöcke zu entfernen, sofern der Feldblock nicht leer ist
       * und er sich nicht mehr im Fall befindet */
      if (fieldBlocks[x][y] != 0
          && !(y+1 < FIELD_ROWS && fieldBlocks[x][y+1] == 0)) {
        removed = removed + removeBlocksByCoor(x, y);
      }
    }
  }

  return removed;
}

/*
 * Bewege alle Feldblöcke, sofern dabei Blöcke entfernt werden,
 * wird dies als Integer-Wert zurück gegeben.
 */
int moveAllFieldBlocks() {
  // Derzeit fallen Blöcke = false, wird ggf. auf true gesetzt
  hasFallingBlocks = false;
  int removedBlocks = 0;

  // Setzt alle Blockfelder um eins hinunter (sofern möglich),
  // dabei wird von unten nach oben gelesen
  for (int x=FIELD_COLS-1; x > -1; x--) {
    for (int y=FIELD_ROWS-1; y > 0; y--) {

      // Sofern das obere Feld belegt ist und das untere frei, setze es hinunter
      if (fieldBlocks[x][y] == 0 &&
          fieldBlocks[x][y-1] > 0) {

        fieldBlocks[x][y] = fieldBlocks[x][y-1];
        fieldBlocks[x][y-1] = 0;

        // Sofern sich der Block innerhalb der Blockgruppe befindet, ändere dessen Wert
        int containerIdx = getBlockContainerIndex(x, y-1);
        if (containerIdx > -1) {
          blockContainer[containerIdx][1] = y;
        }

        // Sofern das untere Feld noch frei ist, müssen noch weitere Blöcke fallen
        if (y+1 < FIELD_ROWS && fieldBlocks[x][y+1] == 0) {
          hasFallingBlocks = true;

        } else if (getBlockContainerIndex(x, y) == -1 &&
                   !(y+1 < FIELD_ROWS && getBlockContainerIndex(x, y+1) != -1)) {
          // ansonsten wenn das feld + nächstes nicht mehr
          // zu der blockgruppe gehört, wird versucht gleichfarbige felder zu entfernen
          removedBlocks = removeBlocksByCoor(x, y);
        }
      }
    }
  }

  // Überprüft noch, ob der Block-Container noch intakt ist und setzt in ggf. neu
  if (getBlockContainerState() == 0) {
    resetBlockContainer();
  }

  return removedBlocks;
}

/*
 * Drehe einen einzelnen Block oder überprüfe nur,
 * ob das Drehen des Block-Containers überhaupt möglich ist.
 * Siehe auch rotateBlockContainer.
 */
int rotateBlock(int i, int rotation, int mX, int mY, int possible, bool checkOnly) {
  int newX = -1;
  int newY = -1;

  if (blockContainer[i][0] != mX || blockContainer[i][1] != mY) {

    if (blockContainer[i][0] == mX) {
      // x-Werte stimmen überein
      newY = blockContainer[i][1] + ((mY - blockContainer[i][1]));
      newX = blockContainer[i][0] + ((mY - blockContainer[i][1]) * rotation);

    } else if (blockContainer[i][1] == mY) {
      // y-Werte stimmen überein
      newX = blockContainer[i][0] + ((mX - blockContainer[i][0]));
      newY = blockContainer[i][1] + ((mX - blockContainer[i][0]) * -rotation);
    }

    // Überprüfe nur, anstatt zu drehen
    if (checkOnly == true) {
      // Überprüfe ob der Feldblock schon belegt ist, und dieser im gültigen Bereich ist
      if ((newX < 0 || newX >= FIELD_COLS || newY < 0 || newY >= FIELD_ROWS)
          || fieldBlocks[newX][newY] > 0) {
        possible = 0;
      }

    } else {
      // Drehe Block
      fieldBlocks[newX][newY] = fieldBlocks[blockContainer[i][0]][blockContainer[i][1]];
      fieldBlocks[blockContainer[i][0]][blockContainer[i][1]] = 0;
      // Und referenziere die neue Position im Container
      blockContainer[i][0] = newX;
      blockContainer[i][1] = newY;
    }
  }

  if (i+1 < BLOCK_CONTAINER_SIZE) {
    // sofern noch Blöcke bestehen prüfe / verschiebe auch diese
    possible = rotateBlock(i+1, rotation, mX, mY, possible, checkOnly);
  }

  return possible;
}

/*
 * Überprüft ob das Drehen eines Block-Containers überhaupt möglich ist, dazu wird
 * überprüft, ob alle Zielfeldblöcke noch nicht belegt sind.
 * Danach wird der Container gedreht.
 */
bool rotateBlockContainer(int rotation) {

  if (hasBlockContainer == true) {
    // Ermittle mittleren Block als Rotationsbasis
    int middle[2];
    middle[0] = blockContainer[(int)floor(BLOCK_CONTAINER_SIZE/2)][0];
    middle[1] = blockContainer[(int)floor(BLOCK_CONTAINER_SIZE/2)][1];

    // Gehe durch die Container-Elemente um deren referenzierte Zielkoordinaten zu validieren
    int possible = 1;
    possible = rotateBlock(0, rotation, middle[0], middle[1], 1, true); // rekursiv

    if (possible == 1) {
      // Geht durch die Container-Elemente um die referenzierten Blöcke zu verschieben
      rotateBlock(0, rotation, middle[0], middle[1], 1, false); // rekursiv

      playSound(1); // Sound beim Rotieren
      return true;
    }
  }
  return false;
}

/*
 * Diese rekursive Funktion ist für moveBlockGroupToSide gedacht, da sie einzelne
 * Blöcke überprüft / bzw verschieben kann. Dabei werden alle Blöcke durch
 * rekursive Verarbeitung betrachtet / verschoben.
 * Nachdem die erste Instanz dieser Funktion abgeschlossen wurde, sollten die
 * Hilfsflags des Block-Containers zurück gesetzt werden.
 */
int moveBlockToSide(int i, int xDirection, int moved, bool checkOnly) {

  // index ist valide
  if (i > -1 && i < BLOCK_CONTAINER_SIZE) {

    // new x ist valide
    int newX = blockContainer[i][0] + xDirection;

    // ermittle den Index im Block-Container anhand des neuen x-Werts (falls mögl.)
    int newBlockIndex = getBlockContainerIndex(newX, blockContainer[i][1]);

    // Ermittelt ob schon ein fremder Block auf dem Feld ist
    int ungroupedBlock = false;
    if (newX > -1 && newX < FIELD_COLS
        && fieldBlocks[newX][blockContainer[i][1]] != 0
        && newBlockIndex == -1) {
      ungroupedBlock = true;
    }

    // Neuer x wert ist innerhalb des Felds und auch nicht durch einen fremden Block belegt
    if (newX > -1 && newX < FIELD_COLS && ungroupedBlock == false) {

      // Feld wird durch einen anderen eigenen Block belegt
      if (newBlockIndex > i && blockContainer[newBlockIndex][2] == -1) {
        moved = moveBlockToSide(newBlockIndex, xDirection, moved, checkOnly);
      }

      // Block wurde noch nicht verarbeitet
      if (blockContainer[i][2] == -1) {
        // Verschiebe sofern gewünscht
        if (checkOnly == false) {
          fieldBlocks[newX][blockContainer[i][1]] = fieldBlocks[blockContainer[i][0]][blockContainer[i][1]];
          fieldBlocks[blockContainer[i][0]][blockContainer[i][1]] = 0;
          blockContainer[i][0] = newX;
        }
        // setze ein Flag, um den Block nicht noch einmal zu verschieben / checken
        blockContainer[i][2] = 1;
        moved++;
      }

    }

    // Es gibt noch weitere Container-Elemente? Sofern noch kein ungroupedBlock besteht
    if (ungroupedBlock == false
        && i+1 < BLOCK_CONTAINER_SIZE && blockContainer[i+1][2] == -1) {
      moved = moveBlockToSide(i+1, xDirection, moved, checkOnly);
    }
  }

  return moved;
}

/*
 * Überprüft und verschiebt einen Block-Container und desse ref. Blöcke seitlich.
 */
bool moveBlockContainerToSide(int direction) {

  if (hasBlockContainer == true) {
    // überprüfe die referenzierten Blöcke zum Verschieben
    int checked = moveBlockToSide(0, direction, 0, true); // rekursiv
    // setze "überprüft"-flags zurück
    for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
      blockContainer[i][2] = -1;
    }

    if (checked == BLOCK_CONTAINER_SIZE) {
      // verschiebe Blöcke
      moveBlockToSide(0, direction, 0, false); // rekursiv
      // setze "verschoben"-Flags zurück
      for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
        blockContainer[i][2] = -1;
      }

      playSound(0); // Sound beim Verschieben
      return true;
    }
  }
  return false;
}

/*
 * Bewegt den Block-Container nach unten.
 */
bool moveBlockContainerDownwards() {

  if (hasBlockContainer == true) {
    int moveable = true;
    int idxToStartMovement = -1;
    int containerIdx = -1;

    if (getBlockContainerState() > 0) {

      // Ermittle ob sich noch Platz unter dem Block-Container befindet
      for (int i=BLOCK_CONTAINER_SIZE-1; i > -1; i--) {
        /* Sofern sich das untere Feld nicht innerhalb des Block-Containers befindet,
         * es nicht belegt ist, und es im Spielfeld ist -> überprüfen */
        containerIdx = getBlockContainerIndex(blockContainer[i][0], blockContainer[i][1]+1);
        if (containerIdx == -1
            && fieldBlocks[blockContainer[i][0]][blockContainer[i][1]+1] == 0
            && blockContainer[i][1]+1 < FIELD_ROWS) {
          idxToStartMovement = i; // merkt sich, bei welchem Block er dann später beginnen muss

        } else if (containerIdx == -1) {
          // Verhindert das Bewegen, wenn ein Problem auftritt, und der nächste Block kein Eigener ist
          moveable = false;
        }
      }

      if (moveable == true) {
        // Die Bewegung kann nur vom ersten oder letzten Container-Element aus starten
        if (idxToStartMovement == 0) {
          for (int i=0; i < BLOCK_CONTAINER_SIZE; i++) {
            // Feldblock-Farbe ändern
            int color = fieldBlocks[blockContainer[i][0]][blockContainer[i][1]];
            fieldBlocks[blockContainer[i][0]][blockContainer[i][1]] = 0;
            fieldBlocks[blockContainer[i][0]][blockContainer[i][1]+1] = color;
            // Feldblock-Position ändern
            blockContainer[i][1] = blockContainer[i][1] + 1;
          }
          playSound(0); // Sound beim Verschieben
          return true;

        } else if (idxToStartMovement == BLOCK_CONTAINER_SIZE-1) {
          for (int i=BLOCK_CONTAINER_SIZE-1; i > -1; i--) {
            // Feldblock-Farbe ändern
            int color = fieldBlocks[blockContainer[i][0]][blockContainer[i][1]];
            fieldBlocks[blockContainer[i][0]][blockContainer[i][1]] = 0;
            fieldBlocks[blockContainer[i][0]][blockContainer[i][1]+1] = color;
            // Feldblock-Position ändern
            blockContainer[i][1] = blockContainer[i][1] + 1;
          }
          playSound(0); // Sound beim Verschieben
          return true;
        }
      }
    }
  }

  return false;
}

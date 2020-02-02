#include "display.h"

#include <iostream>

using namespace std;


// ##############################################################################
// #####                        CURSOR-FUNKTIONEN                           #####
// ##############################################################################

// Cursor um n Zeilen nach oben bewegen.
void moveCursorUp(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "A";
} // moveCursorUp.

// Cursor um n Zeilen nach unten bewegen.
void moveCursorDown(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "B";
} // moveCursorDown.

// Cursor um n Spalten nach links bewegen.
void moveCursorLeft(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "D";
} // moveCursorLeft.

// Cursor um n Spalten nach rechts bewegen.
void moveCursorRight(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "C";
} // moveCursorRight.


// Loescht den Inhalt des Displays und springt an den Beginn der Zeile.
void clearScreen() {
    cout << "\033[2J" << endl;
} // clearScreen.


void moveUp() {
    moveCursorUp();
}
void moveDown() {
    moveCursorDown();
}
void moveLeft() {
    moveCursorLeft();
}
void moveRight() {
    moveCursorRight();
}
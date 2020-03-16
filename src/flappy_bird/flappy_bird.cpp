#include <iostream>

#include "pitendo_game_engine.h"
#include "flappy_bird.h"


using namespace std;
using namespace flappyBird;


// ##############################################################################
// #####                    SPIELOBJEKTE DEFINIEREN                         #####
// ##############################################################################

// Unser Spielfeld.
PlayingField *playground = new PlayingField();



// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Diese Funktion wird einmalig aufgerufen, wenn im Hauptmenue das Spiel
// angewaehlt wird. 
void flappyBird::gameStart() {
    // Hauptmenue korrekt beenden.
    mainMenu::menuExit();

    // Options-Menue an das Spiel anpassen.
    if (pitendoGE->customiseOptionMenu(&flappyBird::gameReturn, &flappyBird::gameStop) == false) {
        return;
    }


    // Spiel-Bildschirm darstellen.
    clearScreen();
    playground->addPipe();    


    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();


    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunction = &flappyBird::gameHandle;

} // flappyBird::gameStart.


// Diese Funktion wird bei laufendem Spiel in jedem Schleifendurchlauf aufgerufen.
void flappyBird::gameHandle() {
    // Soll pausiert werden?
    if (controllerP1->buttonStart->wasPressed() || controllerP2->buttonStart->wasPressed()) {
        flappyBird::gamePause();
        return;
    }

    if (++playground->timeNow >= playground->timePerMovement) {
        playground->moveLeft();
        playground->timeNow = 0;
    }

} // flappyBird::gameHandle.


// Diese Funktion wird aufgerufen, wenn im Spiel "Pause" gedrueckt wird. 
void flappyBird::gamePause() {
    // Zwischenstand abspeichern?
    // ... in diesem Fall nicht.


    // Options-Menue anzeigen.
    optionMenu::menuStart();

} // flappyBird::gamePause.


// Diese Funktion wird aufgerufen, wenn aus dem Pausenmenue zurueck zum Spiel
// gekehrt wird.
void flappyBird::gameReturn() {
    // Option-Menue korrekt beenden.
    optionMenu::menuExit();

    // Spiel-Bildschirm darstellen.
    clearScreen();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunction = &flappyBird::gameHandle;

} // flappyBird::gameReturn.


// Diese Funktion wird aufgerufen, wenn das Spiel beendet wird und zurueck zum
// Hauptmenue gekehrt wird. Es koennen final Spielstaende gesichert werden.
void flappyBird::gameStop() {
    // Muessen Spielstaende gespeichert werden?
    // ... in diesem Fall nicht. 

    // Muss Speicher wieder freigegeben werden?
    // ... in diesem Fall nicht. 

    // Verwerfe im Optionen-Menue die spielabhaengigen Eintraege.
    if (pitendoGE->resetOptionMenu() == false) {
        return;
    }

    // Rufe wieder das Hauptmenue auf. Das darf nicht entfernt werden,
    // da sonst aus dem Spiel nicht mehr ins Hauptmenue zurueckgekehrt werden kann.
    mainMenu::menuStart();

} // flappyBird::gameStop.



// ##############################################################################
// #####                    INDIVIDUELLE FUNKTIONEN                         #####
// ##############################################################################


// ##############################################################################
// #####                      KLASSE PLAYINGFIELD                           #####
// ##############################################################################


// Konstruktor Klasse PlayingField.
PlayingField::PlayingField() {
    // Anzahl von Rohren.
    this->numberOfPipes = 0;

    // Horizontaler Abstand der Rohre.
    this->pipeDistance = 0;

    // Vertikaler maximaler Abstand der Rohrdurchgaenge aufeinanderfolgender Rohre.
    this->maxPipeGateDistance = 0;

    // Timing.
    this->timeNow = 0;
    this->timePerMovement = 2;
} // Konstruktor Klasse PlayingField.


// Destruktor Klasse PlayingField.
PlayingField::~PlayingField() {

} // Destruktor Klasse PlayingField.


// Funktion zum Hinzufuegen eines neuen Rohres.
void PlayingField::addPipe() {
    this->vectorPipes.push_back(Pipe(30));
    this->numberOfPipes++;
} // PlayingField::addPipe.


// Funktion zum Zeichnen des Spielfeldes am Bildschirm.
void PlayingField::draw() {
    // Zeichne jedes Rohr.
    for (int i = 0; i < this->numberOfPipes; i++) {
        vectorPipes[i].draw();
    }
} // PlayingField::draw.


// Funktion zum Weiterbewegen des Spielfeldes.
void PlayingField::moveLeft() {
    // Schiebe alles nach links. Ggf. muss ein Rohr geloescht werden.
    // Hinweis: Das Programm geht in die Knie, wenn mehrere Rohre gleichzeitig das
    // Ende erreichen. Das ist zu verhindern, da hier keine Fehlerbehandlung stattfindet.
    int posDelete = -1;
    for (int i = 0; i < this->numberOfPipes; i++) {
        if (this->vectorPipes[i].moveLeft() == false) {
            // Rohr ist am linken Bildschirm-Rand angekommen und wurde bereits nicht
            // mehr gezeichnet.
            // Loesche das Handle auf das Rohr und gib zuvor noch den Speicher frei.
            delete &this->vectorPipes[i];
            posDelete = i;
        }
    }
    // Handle loeschen.
    if (posDelete != -1) {
        this->vectorPipes.erase(this->vectorPipes.begin() + posDelete);
        this->numberOfPipes--;
    }
} // PlayingField::moveLeft.


// Funktion zum Checken, ob eine Kollision vorliegt.
bool PlayingField::checkForCollision(int posX1, int posX2, int posY1, int posY2) {
    return false;
} // PlayingField::checkForCollision.


// ##############################################################################
// #####                         KLASSE PIPE                                #####
// ##############################################################################


// Konstruktor Klasse Pipe.
PlayingField::Pipe::Pipe(int posY) {
    // Position der Rohre in x-Richtung. Diese Angabe bezieht sich auf
    // den am weitesten links liegenden Punkt der Rohre.
    this->posX = pitendoGE->screenWidth - 8;

    // Position des Durchganges in y-Richtung. Diese Angabe bezieht sich auf
    // das Ende des oberen Rohres.
    this->posY = posY;

    // Hoehe des Durchganges in y-Richtung.
    this->gateHeight = 20;

    // Breite der Rohre in x-Richtung.
    this->pipeWidth = 5;
} // Konstruktor Klasse Pipe.


// Destruktor Klasse Pipe.
PlayingField::Pipe::~Pipe() {

} // Destruktor Klasse Pipe.


// Funktion zum Zeichnen der beiden Rohre am Bildschirm.
// (inkl. Loeschen der alten gemalten Position)
void PlayingField::Pipe::draw() {
    for (int posY = 1; posY <= pitendoGE->screenHeight; posY++) {
        if ((posY == this->posY) || (posY == (this->posY + this->gateHeight + 1))) {
            // Rohr-Enden.
            setCursorPosition(this->posX, posY);
            cout << "[";
            for (int i = 0; i < this->pipeWidth - 2; i++) cout << "=";
            cout << "] ";   // Mit Leerzeichen am Ende, um vorherige Position loeschen zu koennen.
        }
        else if ((posY < this->posY) || (posY > (this->posY + this->gateHeight + 1))) {
            // Rohr.
            setCursorPosition(this->posX, posY);
            cout << "|";
            for (int i = 0; i < this->pipeWidth - 2; i++) cout << " ";
            cout << "| ";   // Mit Leerzeichen am Ende, um vorherige Position loeschen zu koennen.
        }
    }
} // PlayingField::Pipe::draw.


// Funktion zum Loeschen des Rohres.
void PlayingField::Pipe::remove() {
    for (int posY = 1; posY <= pitendoGE->screenHeight; posY++) {
        setCursorPosition(this->posX, posY);
        for (int i = 0; i < this->pipeWidth; i++) cout << " "; 
    }
} // PlayingField::Pipe::remove.


// Funktion zum Checken, ob eine Kollision vorliegt.
bool PlayingField::Pipe::checkForCollision(int posX1, int posX2, int posY1, int posY2) {
    return false;
} // PlayingField::Pipe::checkForCollision.


// Weiterbewegung des Rohres (inkl. Loeschen der alten Position).
// Der Rueckgabewert ist true, wenn weiter geschoben werden
// konnte und ist false, wenn das Ende des Bildschirms erreicht ist
// und somit das Rohr geloescht werden kann.
// Im Falle des Erreichens des Randes des Bildschirms, loescht sich das
// Rohr selber von dem Bildschirm.
bool PlayingField::Pipe::moveLeft() {
    if (this->posX == 1) {
        // Spielfeldrand erreicht. Lebensdauer des Rohres ist erreicht.
        this->remove();
        return false;
    }
    else {
        // Rohr kann noch weiter nach links verschoben werden.
        this->posX--;
        this->draw();
        return true;
    }
} // PlayingField::Pipe::moveLeft.
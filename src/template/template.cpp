#include <iostream>

#include "pitendo_game_engine.h"    // beinhaltet auch display.h und controller.h
#include "template.h"                                                   // ##### MUSS ANGEPASST WERDEN (header-file) #####


using namespace std;

// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################

// Objektdefinition.
// Spielball-Objekt.
gameTemplate::Ball *spielball1, *spielball2;

// Diese Funktion wird einmalig aufgerufen, wenn im Hauptmenue das Spiel
// angewaehlt wird. 
void gameTemplate::gameStart() {                                        // ##### MUSS ANGEPASST WERDEN (namespace) #####
    // Options-Menue an das Spiel anpassen.
    if (pitendoGE->customiseOptionMenu(&gameTemplate::gameReturn, &gameTemplate::gameStop) == false) {  // ##### MUSS ANGEPASST WERDEN (namespace) #####
        return;
    }

    // Spiel-Bildschirm darstellen.                                     // ##### MUSS ANGEPASST WERDEN #####
    clearScreen();
    
    // Benoetigte Objekte erstellen.
    spielball1 = new Ball(10, 10);
    spielball2 = new Ball(20, 10);

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunktion = &gameTemplate::gameHandle;

} // gameTemplate::gameStart.


// Diese Funktion wird bei laufendem Spiel in jedem Schleifendurchlauf aufgerufen.
void gameTemplate::gameHandle() {                                       // ##### MUSS ANGEPASST WERDEN (namespace) #####
    // Soll pausiert werden?
    if (controllerP1->buttonStart->wasPressed() || controllerP2->buttonStart->wasPressed()) {
        gameTemplate::gamePause();
        return;
    }

    // Ermittle Joystick-Position.
    float x1, y1, x2, y2;
    controllerP1->joystick->getPosition(x1, y1);
    controllerP2->joystick->getPosition(x2, y2);

    // Bewege die Baelle.
    spielball1->move((int)(x1 * 3.0f), (int)(y1 * 3.0f));
    spielball2->move((int)(x2 * 3.0f), (int)(y2 * 3.0f));

} // gameTemplate::gameHandle.


// Diese Funktion wird aufgerufen, wenn im Spiel "Pause" gedrueckt wird. 
void gameTemplate::gamePause() {                                        // ##### MUSS ANGEPASST WERDEN (namespace) #####
    // Zwischenstand abspeichern?
    // ... in diesem Fall nicht.


    // Options-Menue anzeigen.
    optionMenu::menuStart();
} // gameTemplate::gamePause.


// Diese Funktion wird aufgerufen, wenn aus dem Pausenmenue zurueck zum Spiel
// gekehrt wird.
void gameTemplate::gameReturn() {                                       // ##### MUSS ANGEPASST WERDEN (namespace) #####
    // Spiel-Bildschirm darstellen.                                     // ##### MUSS ANGEPASST WERDEN #####
    clearScreen();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunktion = &gameTemplate::gameHandle;


} // gameTemplate::gameReturn.


// Diese Funktion wird aufgerufen, wenn das Spiel beendet wird und zurueck zum
// Hauptmenue gekehrt wird. Es koennen final Spielstaende gesichert werden.
void gameTemplate::gameStop() {                                         // ##### MUSS ANGEPASST WERDEN (namespace) #####
    // Muessen Spielstaende gespeichert werden?
    // ... in diesem Fall nicht.                                        // ##### MUSS ANGEPASST WERDEN (falls notwendig) #####

    // Muss Speicher wieder freigegeben werden?                         // ##### MUSS ANGEPASST WERDEN (falls notwendig) #####
    delete spielball1;
    delete spielball2;                                    

    // Verwerfe im Optionen-Menue die spielabhaengigen Eintraege.
    if (pitendoGE->resetOptionMenu() == false) {
        return;
    }

    // Rufe wieder das Hauptmenue auf. Das darf nicht entfernt werden,
    // da sonst aus dem Spiel nicht mehr ins Hauptmenue zurueckgekehrt werden kann.
    mainMenu::menuStart();

} // gameTemplate::gameStop.



// ##############################################################################
// #####                    INDIVIDUELLE FUNKTIONEN                         #####
// ##############################################################################

// Ball-Konstruktor.
gameTemplate::Ball::Ball(int posX, int posY) {
    this->posX = posX;
    this->posY = posY;
}

// Destruktor.
gameTemplate::Ball::~Ball() {
    // Mach nichts.
}

// Bewegung.
void gameTemplate::Ball::move(int speedX, int speedY) {
    // Ball an alter Position loeschen.
    setCursorPosition(this->posX, this->posY);
    cout << " ";
    // Ball an neuer Position zeichnen.
    this->posX = this->posX + speedX;
    this->posY = this->posY - speedY;   // Negativ aufgrund Pixelzaehlrichtung (1,1 = oben links.)
    // Falls ausserhalb des zulaessigen Bereiches, dann Werte manuell setzen.
    if (this->posX <= 0) this->posX = 1;
    if (this->posX >= pitendoGE->screenWidth) this->posX = pitendoGE->screenWidth;
    if (this->posY <= 0) this->posY = 1;
    if (this->posY >= pitendoGE->screenHeight) this->posY = pitendoGE->screenHeight;
    setCursorPosition(this->posX, this->posY);
    cout << "o";
}

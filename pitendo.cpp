#include <iostream>
#include <wiringPi.h>
#include "controller.h"
#include "display.h"
#include "pitendo_game_engine.h"

using namespace std;


int main(int argc, char** argv) {
    // ##############################################################################
    // #####                        PITENDO-SETUP                               #####
    // ##############################################################################

    // Bildschirm initialisieren.
    if (argc == 1) {
        // Dies ist der Aufruf durch den Nutzer.
        // Initialisiere Pitendo (ueberpruefe Abhaengigkeiten und starte Pitendo in einem eigenen
        // Terminal erneut).
        if (pitendoInit() == true) {
            cout << "Pitendo erfolgreich initialisiert." << endl;
            cout << "Pitendo wird in einem separaten Fenster gestartet." << endl;
            return 1;
        }
        else { 
            cerr << "Pitendo konnte nicht initialisiert werden." << endl;
            return -1;
        }
    }

    // Pitendo final vorbereiten und einrichten.
    if (pitendoSetup() == true) {
        cout << "Pitendo erfolgreich eingerichtet." << endl;
    }
    else {
        cerr << "Pitendo konnte nicht eingerichtet werden." << endl;
        cout << "ENTER zum Beenden druecken ..." << endl;
        cin.get(); // Nutzereingabe abwarten, damit Nutzer Zeit hat, Fehlermeldung zu lesen.
        return -1;
    }


    // ##############################################################################
    // #####                    SPIELE-REGISTRIERUNG                            #####
    // ##############################################################################




    // ##############################################################################
    // #####                    PITENDO-DAUERSCHLEIFE                           #####
    // ##############################################################################

    // Buttonfunktionen setzen.
    controllerP1->buttonGruen->buttonFunktion = &clearScreen;
    controllerP1->buttonRot->buttonFunktion = &pitendoExit;
    controllerP2->buttonRot->buttonFunktion = &moveDown;
    controllerP2->buttonBlau->buttonFunktion = &moveLeft;
    controllerP2->buttonGelb->buttonFunktion = &moveRight;

    float x, y;
    // Dauerschleife.
    while(pitendoGE->isRunning == true) {
        controllerP1->execute();
        controllerP2->execute();
        controllerP1->joystick->getPosition(x, y);
        if (x < -0.5) {
            moveLeft();
        }
        else if (x > 0.5) {
            moveRight();
        }
        if (y < -0.5) {
            moveDown();
        }
        else if (y > 0.5) {
            moveUp();
        }
        cout << flush;
        delay(20);
    }

    // ##############################################################################
    // #####                    ABSCHLIESSENDE HANDLUNGEN                       #####
    // ##############################################################################

    // Dauerschleife wurde verlassen. Pitendo soll beendet werden.
    pitendoDestroy();

    return 1;
}
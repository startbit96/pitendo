#include <iostream>
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
        cout << "Zum Beenden ENTER druecken ..." << endl;
        cin.get(); // Nutzereingabe abwarten, damit Nutzer Zeit hat, Fehlermeldung zu lesen.
        return -1;
    }



    // ##############################################################################
    // #####                    SPIELE-REGISTRIERUNG                            #####
    // ##############################################################################




    // ##############################################################################
    // #####                    PITENDO-DAUERSCHLEIFE                           #####
    // ##############################################################################


    // Willkommens-Bildschirm.
    pitendoStartScreen();

    // Dauerschleife.
    while(pitendoGE->isRunning == true) {
        // Die der Game-Engine hinterlegte Funktion ausfuehren.
        pitendoGE->gameEngineFunktion();

        // Display aktualisieren.
        cout << flush;
        
        // System pausieren (fps einstellen).
        // MUSS NOCH IN DIE GAMEENGINE INTEGRIERT WERDEN.
        delay(20);
    }



    // ##############################################################################
    // #####                    ABSCHLIESSENDE HANDLUNGEN                       #####
    // ##############################################################################

    // Dauerschleife wurde verlassen. Pitendo soll beendet werden.
    pitendoDestroy();

    return 1;
}
#include <iostream>
#include <fstream>
#include <wiringPi.h>
#include "controller.h"
#include "display.h"


using namespace std;

void rot1() {
    cout << "rot 1" << endl;
}
void rot2() {
    cout << "rot 2" << endl;
}
void blau1() {
    cout << "blau 1" << endl;
}

int main(int argc, char** argv) {
    int i = 0;

    // Bildschirm initialisieren.
    if (argc == 1) {
        // Dies ist der Aufruf durch den Nutzer.
        // Erstelle daher ein neues xterm-Fenster und starte Pintendo.

        // Ist auf dem Raspberry Pi xterm installiert?
        if (FILE *file = fopen("/usr/bin/xterm", "r")) {
            fclose(file);
            cerr << "Auf dem Raspberry Pi ist kein xterm installiert." << endl;
            cerr << "Installiere erst mittels \"sudo apt-get install xterm\" den Terminal-Emulator." << endl;
            return -1;
        }
        else {
            // Starte Pintendo in einem neuen Display und beende das Programm auf diesem Terminal.
            
        }   

    }

    else {

    }

    // Controller initialisieren.
    if (controllerSetup() == true) {
        cout << "Controller erfolgreich initalisiert." << endl;
    }
    else {
        cout << "Controller konnte nicht initialisiert werden." << endl;
    }

    // Buttonfunktionen setzen.
    controllerP1->buttonRot->buttonFunktion = &rot1;
    controllerP1->buttonBlau->buttonFunktion = &blau1;
    controllerP2->buttonRot->buttonFunktion = &rot2;

    // Display initialisieren.
    while(true) {
        controllerP1->execute();
        controllerP2->execute();
        delay(20);
    }

    // Hauptmenue aufrufen.

    // Lets play.
}
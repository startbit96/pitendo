#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
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

void moveUp() {
    cout << "\033[A";
}

void moveDown() {
    cout << "\033[B";
}

void moveRight() {
    cout << "\033[C";
}

void moveLeft() {
    cout << "\033[D";
}

void writeToTerminal() {
    cout << "W";
}

void changeFontSize() {
    //echo -e "\033]50;DejaVu Sans Mono:pixelsize=16\a"
    //system("echo -n \"\e]50;#0\a\"");
}


int main(int argc, char** argv) {
    int i = 0;

    // Bildschirm initialisieren.
    if (argc == 1) {
        // Dies ist der Aufruf durch den Nutzer.
        // Erstelle daher ein neues xterm-Fenster und starte Pintendo.
        // Systemvariable setzen, damit die Ausgabe auf dem Bildschirm des Raspberry Pi geschieht.
        // Dies ist bei Remote-Development ueber Visual Studio Code notwendig.
        setenv("DISPLAY", ":0", 1);
        delay(100);

        // Ist auf dem Raspberry Pi xterm installiert?
        if (FILE *file = fopen("/usr/bin/xterm", "r")) {
            fclose(file);
            cout << "xterm gefunden." << endl;
        }
        else {
            cerr << "Auf dem Raspberry Pi ist kein xterm installiert." << endl;
            cerr << "Installiere erst mittels \"sudo apt-get install xterm\" den Terminal-Emulator." << endl;
            return -1;
        }

        // Ist die Schrift installiert?
        if (FILE *file = fopen("/home/pi/.fonts/square.ttf", "r")) {
            fclose(file);
            cout << "square gefunden." << endl;
        }
        else {
            cerr << "Auf dem Raspberry Pi ist kein square installiert." << endl;
            return -1;
        }

        // Neues Terminal oeffnen und pintendo ausfuehren.
        system("xterm -fa \"square:size=8:antialias=true\" -fullscreen -e \"./build/pintendo start\"");
        return 1;
    }
    else {
    }

    // Cursor ausschalten.
    //system("setterm -cursor off");
    // System kurz zur Ruhe kommen lassen, da sonst aus irgendwelchen Gruenden durch die 
    // Fullscreen-Einstellung die tatsaechliche Terminalgroesse dem System noch nicht bekannt ist.
    delay(100);

    // Groesse ermitteln.
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    cout << "Row: " << w.ws_row << endl;
    cout << "Col: " << w.ws_col << endl;

    // Controller initialisieren.
    if (controllerSetup() == true) {
        cout << "Controller erfolgreich initalisiert." << endl;
    }
    else {
        cout << "Controller konnte nicht initialisiert werden." << endl;
    }

    // Buttonfunktionen setzen.
    controllerP2->buttonGruen->buttonFunktion = &moveUp;
    controllerP2->buttonRot->buttonFunktion = &moveDown;
    controllerP2->buttonBlau->buttonFunktion = &moveLeft;
    controllerP2->buttonGelb->buttonFunktion = &moveRight;
    controllerP2->buttonStart->buttonFunktion = &writeToTerminal;
    controllerP1->buttonStart->buttonFunktion = &changeFontSize;

    float x, y;

    // Dauerschleife.
    while(true) {
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

    // Hauptmenue aufrufen.

    // Lets play.
}
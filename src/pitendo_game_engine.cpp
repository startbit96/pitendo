#include "pitendo_game_engine.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#include <wiringPi.h>

#include "controller.h"
#include "display.h"

using namespace std;

// Unsere Game-Engine fuer Pitendo.
GameEngine* pitendoGE;


// ##############################################################################
// #####                    ALLGEMEINE FUNKTIONEN                           #####
// ##############################################################################



// Initialisierung des Systems.
// Diese Funktion prueft das Vorhandensein der notwendigen Ressourcen (xterm, Schriftart).
// Wenn alles passt, wird das Programm in einem separaten Fenster erneut aufgerufen,
// um die Einstellungen (Vollbild, ...) setzen zu koennen.
bool pitendoInit() {
    // Informiere Nutzer.
    cout << "Ueberpruefe Abhaengigkeiten ..." << endl;

    // Wird ueber Remote-Development programmiert (z.B. Visual Studio Code mit SSH), muss 
    // durch das Setzen einer Systemvariable erst angegeben werden, welcher Display genutzt werden
    // soll. Wir moechten natuerlich den des Raspberry Pi.
    setenv("DISPLAY", ":0", 1);

    // Ist auf dem Raspberry Pi xterm installiert? Dies ist der in dem Projekt Pitendo verwendete Terminal-Emulator.
    if (FILE *file = fopen("/usr/bin/xterm", "r")) {
        // IO.
        fclose(file);
        cout << "Terminal-Emulator xterm ist installiert." << endl;
    }
    else {
        // NIO.
        cerr << "Auf dem Raspberry Pi ist kein xterm installiert." << endl;
        cerr << "Installiere erst mittels \"sudo apt-get install xterm\" den Terminal-Emulator." << endl;
        return false;
    }

    // Fuer die grafische Darstellung im Terminal eignen sich besonders quadratische Schriftarten gut.
    // Quadratisch bedeutet, dass Zeichen gleich hoch wie breit sind.
    // Pruefe daher, ob die Schrift "square" installiert ist.
    if (FILE *file = fopen("/home/pi/.fonts/square.ttf", "r")) {
        // IO.
        fclose(file);
        cout << "Schriftart square gefunden." << endl;
    }
    else {
        // NIO.
        cerr << "Auf dem Raspberry Pi ist kein square installiert." << endl;
        cerr << "Siehe README.md fuer mehr Informationen zur Installation." << endl;
        return false;
    }

    // Neues Terminal oeffnen und Pitendo mit hoher Prozess-Prioritaet ausfuehren.
    const int pitendoProzessPrioritaet = -20;
    const int fontSize = 6;
    //string consoleCommand = "sudo nice -n " + to_string(pitendoProzessPrioritaet); // Prozess-Prioritaet.
    string consoleCommand = "";
    consoleCommand += "xterm -fa \"square:size=" + to_string(fontSize) + ":antialias=true\""; // Schriftart.
    consoleCommand += " -fullscreen -e \"./build/pitendo start\"";
    cout << "Ausfuehren von \"" << consoleCommand << "\"" << "..." << endl;
    // Konsolen-Kommando ausfuehren.
    system(consoleCommand.c_str());
    return true;
} // pitendoInit;



// Einrichtung der Pitendo-Umgebung.
// Unterschied zu Initialisierung: Initialisierung prueft Abhaengigkeiten. Setup richtet ein.
bool pitendoSetup() {
    // Cursor verstecken.
    //system("setterm -cursor off");

    // System kurz zur Ruhe kommen lassen, da sonst aus irgendwelchen Gruenden durch die 
    // Fullscreen-Einstellung die tatsaechliche Terminalgroesse dem System noch nicht bekannt ist.
    delay(100);

    // Terminal-Groesse ermitteln.
    // !!! Muessen noch irgendwo hinterlegt werden.
    struct winsize windowSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);
    cout << "Fenstergroesse: " << windowSize.ws_row << "x" << windowSize.ws_col << endl;

    // Controller initialisieren.
    if (controllerSetup() == true) {
        cout << "Controller erfolgreich initalisiert." << endl;
    }
    else {
        cerr << "Controller konnte nicht initialisiert werden." << endl;
        return false;
    }

    // Pitendo-Game-Engine erstellen.
    pitendoGE = new GameEngine();

    return true;
} // pitendoSetup.


// Beendet Pitendo.
void pitendoExit() {
    pitendoGE->isRunning = false;
} // pitendoExit.


// Loest alle Abhaengigkeiten auf, damit das Programm sauber beendet werden kann.
void pitendoDestroy() {
    // Aktuell nichts zu tun. Alles was getan werden muss, ist in den Destruktoren der
    // verschiedenen Klassen implementiert und diese werden automatisch aufgerufen.
}




// ##############################################################################
// #####                       KLASSE GAME ENGINE                           #####
// ##############################################################################


// Konstruktor.
GameEngine::GameEngine() {
    this->isRunning = true;
} // GameEngine::GameEngine.


// Destruktor.
GameEngine::~GameEngine() {

} // GameEngine::~GameEngine.

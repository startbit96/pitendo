/*
######################### PITENDO_GAME_ENGINE.H ###############################
Tim Schwarzbrunn

Diese Datei dient der grundlegenden Einrichtung der Game-Engine fuer das Projekt
Pitendo.
Pitendo zeichnet sich durch die Integration eigen entwickelter Controller sowie
der Bereitstellung einer terminalbasierten grafischen Benutzeroberflaeche aus.

###############################################################################
*/

#ifndef _PITENDO_GAME_ENGINE_H_
#define _PITENDO_GAME_ENGINE_H_

#include <wiringPi.h>
#include "controller.h"
#include "display.h"


// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Initialisierung des Systems.
// Diese Funktion prueft das Vorhandensein der notwendigen Ressourcen (xterm, Schriftart).
// Wenn alles passt, wird das Programm in einem separaten Fenster erneut aufgerufen,
// um die Einstellungen (Vollbild, ...) setzen zu koennen.
bool pitendoInit();

// Einrichtung der Pitendo-Umgebung.
// Unterschied zu Initialisierung: Initialisierung prueft Abhaengigkeiten. Setup richtet ein.
bool pitendoSetup();

// Start-Bildschirm.
void pitendoStartScreen();

// Uebergang vom Start-Bildschirm zum Hauptmenue.
// Der Unterschied zum normalen Aufruf des Hauptmenues besteht darin, ggf. noch vorherige Aktivitaeten des 
// Startbildschirms zu beenden (z.B. Startbildschirm-Musik).
void pitendoStart();

// Beendet Pitendo. (raus aus Dauerschleife).
void pitendoExit();

// Loest alle Abhaengigkeiten auf, damit das Programm sauber beendet werden kann.
void pitendoDestroy();



// ##############################################################################
// #####                       KLASSE GAME ENGINE                           #####
// ##############################################################################


class GameEngine {
    public:
        GameEngine(int screenWidth, int screenHeight);   // Konstruktor.
        ~GameEngine();  // Destruktor.

        // Boolscher Wert, der die Hauptdauerschleife steuert.
        // Wird dieser Wert auf false gesetzt, wird Pitendo beendet.
        bool isRunning;

        // Die Funktion hinter diesem Zeiger wird bei jedem Dauerschleifendurchlauf
        // aufgerufen.
        void (*gameEngineFunktion)(void);

        // Eigenschaften des Displays.
        int screenHeight;
        int screenWidth;

        // Haupt- und Optionen-Menue.
        Menu *mainMenu;
        Menu *optionMenu;
    protected:
    private:
        static void defaultGameEngineFunction();    // Funktion fuer die Initialisierung.
}; // Klasse GameEngine.

// Unsere Game-Engine fuer Pitendo.
extern GameEngine* pitendoGE;



// ##############################################################################
// #####                        PITENDO-MENUES                              #####
// ##############################################################################


// Hauptmenue.
namespace mainMenu {
    // Einmaliger Aufruf, welcher intern die Button-Handler auf menueHandler abgibt.
    void menuStart();
    // Dauerhafter Button-Handler.
    void menuHandler();
} // namespace mainMenu.


// Options-Menue mit Eintraegen zur Joystick-Kalibrierung etc.
namespace optionMenu {
    // Einmaliger Aufruf, welcher intern die Button-Handler auf menueHandler abgibt.
    void menuStart();
    // Dauerhafter Button-Handler.
    void menuHandler();
} // namespace optionMenu.




#endif /*!_PITENDO_GAME_ENGINE_H_*/
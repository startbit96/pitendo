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
#include <string>
#include "controller.h"
#include "display.h"


#define DEF_DEFAULT_FPS     30


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

        // Zeitliches Timing in der Dauerschleife.
        // Linux ist kein Echtzeit-Betriebssystem, daher sind die FPS eher als ueber 
        // den Daumen gepeilt anzusehen.
        bool setFPS(int fps);
        void adjustFPS();

        // Eigenschaften des Displays.
        int screenWidth, screenHeight;

        // Haupt- und Optionen-Menue.
        Menu *mainMenu;
        Menu *optionMenu;

        // Hinzufuegen eines neuen Spieles mit Uebergabe der Spielstart-Funktion.
        void addGame(std::string gameName, void (*gameStartFunction)(void));

        // Wenn ein Spiel gestartet wurde, ist auch das Optionen-Menue an die spieleigene
        // Pausenfunktion und Beendenfunktion anzupassen.
        bool customiseOptionMenu(void (*gameReturnFunction)(void), void (*gameStopFunction)(void));
        // Wenn das Spiel verlassen wurde, ist die eben angesprochene Anpassung des
        // Optionenmenues wieder rueckgaengig zu machen.
        bool resetOptionMenu();

    protected:

    private:
        // Funktion fuer die Initialisierung.
        static void defaultGameEngineFunction();

        // Fuer das interne Handling.
        enum enumPitendoState { PITENDO_WAITING,
                                PITENDO_RUNNING};
        enumPitendoState pitendoState;

        // Zeitliches Timing. 
        unsigned int fps;
        unsigned int loopTimeOld;
}; // Klasse GameEngine.

// Unsere Game-Engine fuer Pitendo.
extern GameEngine* pitendoGE;



// ##############################################################################
// #####                        SONDERFUNKTIONEN                            #####
// ##############################################################################

namespace controllerCalibration {
    // Zur Markierung des Status und Tracking der Handlung.
    enum enumCalibrationState { CALIBRATION_START,
                                CALIBRATION_MEAN,
                                CALIBRATION_MIN_MAX,
                                CALIBRATION_READY,
                                CALIBRATION_IDLE,
                                CALIBRATION_EXIT};

    // Klasse fuer die Kalibrierung.
    class CalibrationManager {
        public:
            CalibrationManager(Controller *controller); // Konstruktor.
            ~CalibrationManager();      // Destruktor.
            
            // Kalibrierung auf Controller schreiben.
            bool calibrate();

            // Der zu kalibrierende Controller.
            Controller *controller;

            // Aktuelle Joystick-Werte.
            int adcX, adcY;

            // Kalibrierwerte.
            int adcMeanX, adcMinX, adcMaxX;
            int adcMeanY, adcMinY, adcMaxY;

            // Zur Markierung des Status und Tracking der Handlung.
            enumCalibrationState calibrationState;

            // Verweildauern fuer die einzelnen Stati.
            int timePause, timeMean, timeMinMax;

            // Tracken der Dauer.
            int timeNow;

            // Koordination der Bildschirmausgabe.
            int posY;
        protected:

        private:
    }; // Klasse Calibration-Manager.

    // Kalibrierung des Joysticks. Allgemeine Funktion fuer Dauerschleifenbehandlung.
    void calibrateJoystick();

    // Kalibrierung des Joysticks des ersten Controllers. 
    // Funktion fuer Funktionspointer des Optionen-Menues.
    void calibrateJoystickC1();

    // Kalibrierung des Joysticks des zweiten Controllers. 
    // Funktion fuer Funktionspointer des Optionen-Menues.
    void calibrateJoystickC2();

} // namespace controllerCalibration.


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
#ifndef _PITENDO_GAME_ENGINE_H_
#define _PITENDO_GAME_ENGINE_H_

/*
######################### PITENDO_GAME_ENGINE.H ###############################
Tim Schwarzbrunn

Diese Datei dient der grundlegenden Einrichtung der Game-Engine fuer das Projekt
Pitendo.
Pitendo zeichnet sich durch die Integration eigen entwickelter Controller sowie
der Bereitstellung einer terminalbasierten grafischen Benutzeroberflaeche aus.

###############################################################################
*/


// Initialisierung des Systems.
// Diese Funktion prueft das Vorhandensein der notwendigen Ressourcen (xterm, Schriftart).
// Wenn alles passt, wird das Programm in einem separaten Fenster erneut aufgerufen,
// um die Einstellungen (Vollbild, ...) setzen zu koennen.
bool pitendoInit();

// Einrichtung der Pitendo-Umgebung.
// Unterschied zu Initialisierung: Initialisierung prueft Abhaengigkeiten. Setup richtet ein.
bool pitendoSetup();

// Beendet Pitendo.
void pitendoExit();

// Loest alle Abhaengigkeiten auf, damit das Programm sauber beendet werden kann.
void pitendoDestroy();

// Erzeugen des Hauptmenues.
    // Spiele
    // Optionen
    // Beenden
// Definition eines Pausenmenues.
    // Optionen
    // Zurueck zum Hauptmenue.
    // Pitendo beenden.
// Optionen.
    // Joystick-Kalibrierung.
    // Lautstaerke?

class GameEngine {
    public:
        GameEngine();   // Konstruktor.
        ~GameEngine();  // Destruktor.

        // Boolscher Wert, der die Hauptdauerschleife steuert.
        // Wird dieser Wert auf false gesetzt, wird Pitendo beendet.
        bool isRunning;

        // Die Funktion hinter diesem Zeiger wird bei jedem Dauerschleifendurchlauf
        // aufgerufen.
        void (*gameEngineFunktion)(void);
    protected:

    private:
    
};

// Unsere Game-Engine fuer Pitendo.
extern GameEngine* pitendoGE;


#endif /*!_PITENDO_GAME_ENGINE_H_*/
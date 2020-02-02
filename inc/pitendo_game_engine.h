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

bool pitendoSetup();
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
        bool isRunning;
        void (*gameEngineFunktion)(void);
    protected:

    private:

};

// Unsere Game-Enginge fuer Pitendo.
extern GameEngine* pitendoGE;


#endif /*!_PITENDO_GAME_ENGINE_H_*/
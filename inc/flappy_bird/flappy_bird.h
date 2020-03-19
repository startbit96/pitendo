#ifndef _FLAPPY_BIRD_H_
#define _FLAPPY_BIRD_H_


/*
############################### DISPLAY.H #####################################
Tim Schwarzbrunn

Ein Spiel, in welchem ein Vogel durch eine Landschaft von Rohren navigiert werden
muss. Durch Tippen des gruenen Buttons fliegt der Vogel nach oben. Durch die 
Schwerkraft faellt dieser aber auch wieder nach unten.
Wird ein Rohr beruehrt, ist das Spiel vorbei.

###############################################################################
*/

#include <vector>

#define DEF_PIPE_DISTANCE               45
#define DEF_PIPE_GATE_HEIGHT            24
#define DEF_PIPE_WIDTH                  5
#define DEF_TIME_PER_MOVEMENT           2

#define DEF_PIPE_GATE_Y_WALL_DISTANCE   10
#define DEF_PIPE_GATE_Y_GATE_DISTANCE   50

#define DEF_BIRD_WIDTH                  9
#define DEF_BIRD_HEIGHT                 4
#define DEF_BIRD_ACC_FLYING             -4.0
#define DEF_BIRD_ACC_FALLING            1.0
#define DEF_BIRD_MAX_SPEED              15


namespace flappyBird {


    // ##############################################################################
    // #####                        GRUNDFUNKTIONEN                             #####
    // ##############################################################################

    // Die Grundfunktionen ermoeglichen einen einheitlichen Spiele-Aufbau.

    // Diese Funktion wird einmalig aufgerufen, wenn im Hauptmenue das Spiel
    // angewaehlt wird.
    void gameStart();
    // Diese Funktion wird bei laufendem Spiel in jedem Schleifendurchlauf aufgerufen.
    void gameHandle();
    // Diese Funktion wird aufgerufen, wenn im Spiel "Pause" gedrueckt wird. 
    void gamePause();
    // Diese Funktion wird aufgerufen, wenn aus dem Pausenmenue zurueck zum Spiel
    // gekehrt wird.
    void gameReturn();
    // Diese Funktion wird aufgerufen, wenn das Spiel beendet wird und zurueck zum
    // Hauptmenue gekehrt wird. Es koennen final Spielstaende gesichert werden.
    void gameStop();


    // ##############################################################################
    // #####                    INDIVIDUELLE FUNKTIONEN                         #####
    // ##############################################################################


    class PlayingField {
        public:
            PlayingField();     // Konstruktor.
            ~PlayingField();    // Destruktor.

            // Funktion zum Hinzufuegen eines neuen Rohres.
            void addPipe(int posY);

            // Funktion zum Zeichnen des Spielfeldes am Bildschirm.
            void draw();

            // Funktion zum Weiterbewegen des Spielfeldes.
            void moveLeft();

            // Funktion zum Checken, ob eine Kollision vorliegt.
            bool checkForCollision(int posX1, int posX2, int posY1, int posY2);

            // Eine Klasse fuer die Rohre. Ein Objekt beinhaltet immer sowohl das obere    int posY = this->posY;

            // als auch das untere Rohr.
            class Pipe {
                public:
                    Pipe(int posY);     // Konstruktor.
                    ~Pipe();    // Destruktor.

                    // Funktion zum Zeichnen der beiden Rohre am Bildschirm.
                    // (inkl. Loeschen der alten gemalten Position)
                    void draw();

                    // Funktion zum Loeschen des Rohres.
                    void remove();

                    // Weiterbewegung des Rohres (inkl. Loeschen der alten Position).
                    // Der Rueckgabewert ist true, wenn weiter geschoben werden
                    // konnte und ist false, wenn das Ende des Bildschirms erreicht ist
                    // und somit das Rohr geloescht werden kann.
                    // Im Falle des Erreichens des Randes des Bildschirms, loescht sich das
                    // Rohr selber von dem Bildschirm.
                    bool moveLeft();

                    // Funktion zum Checken, ob eine Kollision vorliegt.
                    // Ueberprueft ausserdem, ob ein Rohr erfolgreich passiert wurde.     
                    enum enumPipeInteraction {
                        PIPE_COLLISION,
                        PIPE_NO_COLLISION,
                        PIPE_PASSED
                    };
                    enumPipeInteraction checkForCollision(int posX1, int posX2, int posY1, int posY2);


                    // Position der Rohre in x-Richtung. Diese Angabe bezieht sich auf
                    // den am weitesten links liegenden Punkt der Rohre.
                    int posX;

                    // Position des Durchganges in y-Richtung. Diese Angabe bezieht sich auf
                    // das Ende des oberen Rohres.
                    int posY;
                protected:

                private:
            }; // Klasse Rohr (Pipe).

            // Vektor von Rohren.
            std::vector<Pipe> vectorPipes;

            // Anzahl von Rohren.
            int numberOfPipes;

            // Anzahl von bereits "bestandenen" Rohren.
            int numberOfPassedPipes;

            // Timing.
            int timeNow;
        protected:

        private:
            
    }; // Klasse Spielfeld (PlayingField).


    // Klasse des zu steuernden Vogels.
    class Bird {
        public:
            Bird();     // Konstruktor.
            ~Bird();    // Destruktor.

            // Funktion zum Zeichnen des Vogels am Bildschirm.
            void draw();

            // Funktion zum Entfernen des Vogels am Bildschirm.
            void remove();

            // Funktion zum Aufsteigen / nach oben fliegen.
            void fly();

            // Funktion zur Neuberechnung der naechsten Position.
            // Ist abhaengig von der Geschwindigkeit in y-Richtung.
            // Einfluss hierauf nehmen die fest definierte Schwerkraft sowie die Nutzer-
            // Eingaben abhaengige Beschleunigung durch das Fliegen.
            void calculatePosition();

            // Position des Vogels erhalten.
            void getPosition(int &posX1, int &posX2, int &posY1, int &posY2);
        protected:

        private:
            // Position in x und y. Diese beziehen sich auf den oberen linken Punkt.
            // posX wird nach Erzeugung nicht mehr veraendet.
            int posX, posY;

            // Geschwindigkeit des Vogels in y-Richtung. Dieser Wert wird durch die auf den Vogel
            // wirkende Beschleunigung (Fluegelschlag / Schwerkraft) dauerhaft beeinflusst.
            float speed;
    }; // Klasse Vogel.

}


#endif /*!_FLAPPY_BIRD_H_*/
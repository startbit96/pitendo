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
            void addPipe();

            // Funktion zum Zeichnen des Spielfeldes am Bildschirm.
            void draw();

            // Funktion zum Weiterbewegen des Spielfeldes.
            void moveLeft();

            // Funktion zum Checken, ob eine Kollision vorliegt.
            bool checkForCollision(int posX1, int posX2, int posY1, int posY2);


            // Timing.
            int timeNow, timePerMovement;
        protected:

        private:
            // Eine Klasse fuer die Rohre. Ein Objekt beinhaltet immer sowohl das obere
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
                    bool checkForCollision(int posX1, int posX2, int posY1, int posY2);
                protected:

                private:
                    // Position der Rohre in x-Richtung. Diese Angabe bezieht sich auf
                    // den am weitesten links liegenden Punkt der Rohre.
                    int posX;

                    // Position des Durchganges in y-Richtung. Diese Angabe bezieht sich auf
                    // das Ende des oberen Rohres.
                    int posY;

                    // Hoehe des Durchganges in y-Richtung.
                    int gateHeight;

                    // Breite der Rohre in x-Richtung.
                    int pipeWidth;
            }; // Klasse Rohr (Pipe).

            // Vektor von Rohren.
            std::vector<Pipe> vectorPipes;

            // Anzahl von Rohren.
            int numberOfPipes;

            // Horizontaler Abstand der Rohre.
            int pipeDistance;

            // Vertikaler maximaler Abstand der Rohrdurchgaenge aufeinanderfolgender Rohre.
            int maxPipeGateDistance;



    }; // Klasse Spielfeld (PlayingField).

/*
    class Bird {
        public:
            Bird(int posX, int posY);     // Konstruktor.
            ~Bird();    // Destruktor.

            // Funktion zum Zeichnen des Vogels am Bildschirm.
            void draw();

            // Funktion zum Aufsteigen / nach oben fliegen.
            void fly();

            // Position des Vogels erhalten.
            getPosition(int &posX1, int &posX2, int &posY1, int &posY2);
        protected:

        private:
            // Position in x und y. Diese beziehen sich auf den oberen linken Punkt.
            int posX, posY;

            // Hoehe und Breite des Vogels.
            int width, height;

            // Geschwindigkeit des Vogels in y-Richtung. Dieser Wert wird durch die auf den Vogel
            // wirkende Beschleunigung (Fluegelschlag / Schwerkraft) dauerhaft beeinflusst.
            float speed;
            float deltaSpeedGravity;
            float deltaSpeedFlying;

            // Zeitraum fuer Unterdrueckung eines weiteren Fluegelschlags.
            int timeNow, timeIdle;

    }; // Klasse Vogel.


    // Zeichnen des Game-Over-Bildschirms.
    void gameOverScreen();
*/

}


#endif /*!_FLAPPY_BIRD_H_*/
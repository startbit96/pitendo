#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_                                                // ##### MUSS ANGEPASST WERDEN #####


/*
############################### TEMPLATE.H ####################################
Tim Schwarzbrunn

Diese Datei dient der Veranschaulichung der Nutzung des Pitendo-Frameworks.
Es bietet sich an, die Datei template.h und die Daten template.cpp zu kopieren
und an den markierten Stellen (##### MUSS ANGEPASST WERDEN #####) entsprechend 
anzupassen.


###############################################################################
*/


// Nutzung eines namespaces ermoeglicht einheitliche Benennung der Grundfunktionen.
// Ausserdem sichert dieser ab, dass Funktionen, welche eventuell von zwei Personen
// gleich benannt wurden, voneinander unterschieden werden koennen.
// Daher bitte alle im spieleigenen Namespace programmieren.
namespace gameTemplate {                                            // ##### MUSS ANGEPASST WERDEN #####

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

    // Die folgenden Funktionen sind fuer jedes Spiel individuell und koennen somit
    // zur Gestaltung des Verhaltens beim Spielen genutzt werden.
    // In dieser Vorlage werden einfach nur Texte am Bildschirm ausgegeben.
    // Ab hier kann somit bei Verwendung des Templates alles weitere im namespace
    // geloescht werden.

    // Beispiel: Fliegender Ball.
    class Ball {
        public:
            Ball(int posX, int posY);     // Konstruktor.
            ~Ball();    // Destruktor.

            // Ball bewegen.
            void move(int speedX, int speedY);
        protected:

        private:
            int posX, posY;
    }; // Klasse Ball.


}


#endif /*!_TEMPLATE_H_*/                                            // ##### MUSS ANGEPASST WERDEN #####
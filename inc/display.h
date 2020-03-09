#ifndef _DISPLAY_H_
#define _DISPLAY_H_


/*
############################### DISPLAY.H #####################################
Tim Schwarzbrunn

Diese Datei dient der Bereitstellung von Funktionalitaeten im Terminal.
Grundlage fuer die Cursor-Operationen sind Escape-Sequenzen.
(siehe z.B. http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html)

Ausserdem wird eine Klasse fuer die Integration von Menues angeboten.

###############################################################################
*/


#include <string>
#include <vector>



// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Cursor-Bewegungen: Relativ.
void moveCursorUp(int numberOfSteps = 1);
void moveCursorDown(int numberOfSteps = 1);
void moveCursorLeft(int numberOfSteps = 1);
void moveCursorRight(int numberOfSteps = 1);


// Cursor-Bewegung: Absolut.
// Hinweis: Es findet keine Ueberpruefung statt, ob die angegebenen Werte innerhalb der eigentlichen Terminalgroesse liegen.
void setCursorPosition(int x, int y);


// Terminal-Fenster komplett loeschen. Der Cursor springt im Anschluss auf die Position {1, 1} (= oben links).
void clearScreen();



// ##############################################################################
// #####                        KLASSE MENUE                                #####
// ##############################################################################

class Menu {
    public:
        Menu(int x_max, int y_max);  // Konstruktor mit Rahmenbegrenzung.
        ~Menu();       // Destruktor.

        // Managen der Eintraege.
        // Hinzufuegen eines neuen Menue-Eintrags. Position gibt an, wo genau der Eintrag angefuegt werden soll.
        void addEntry(std::string text, void (*menuFunction)(void), int position = -1);
        bool deleteEntry(int position);         // Loeschen eines Menue-Eintrags.

        // Bewegung der Auswahl.
        void moveUp();          // Bewegt die Auswahl im Menue um eins nach oben.
        void moveDown();        // Bewegt die Auswahl im Menue um eins nach unten.
        void resetPosition();   // Setzt die Position wieder auf das erste Element.

        // Ausfuehren.
        void execute();         // Aktiviert die hinter der aktuellen Auswahl abgelegte Funktion.

        // Visualisierung. Gibt das Menue am Bildschirm aus.
        void draw();

        // Aktualisiert nur den Marker bei Aenderung der Auswahl im Menue.
        void refresh(int positionOld);

        // Individualisierung. Aendert das Marker-Zeichen.
        bool setMarker(std::string marker);

    protected:

    private:
        // Groesse des Terminals.
        int x_max, y_max;
        // Laenge des laengsten Menue-Textes.
        int maxTextLength;
        // Ermittlung der Zeichen-Nullposition.
        void getMenuPosition(int &x, int &y);
        
        // Handling.
        int numberOfEntries;
        int currentPosition;

        // Individualisierung.
        std::string marker;     // Zeichen, mit dem die Position im Menue markiert wird.

        // Die eigentlichen Eintraege inkl. hinterlegter Funktionen.
        std::vector<std::string> vectorMenuText;
        std::vector<void (*)()> vectorMenuFunctions;

}; // Klasse Menue.


#endif /*!_DISPLAY_H_*/
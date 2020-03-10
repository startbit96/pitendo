#include "display.h"

#include <iostream>

using namespace std;


// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Cursor-Bewegungen: Relativ.
// Cursor um n Zeilen nach oben bewegen.
void moveCursorUp(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "A";
} // moveCursorUp.

// Cursor um n Zeilen nach unten bewegen.
void moveCursorDown(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "B";
} // moveCursorDown.

// Cursor um n Spalten nach links bewegen.
void moveCursorLeft(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "D";
} // moveCursorLeft.

// Cursor um n Spalten nach rechts bewegen.
void moveCursorRight(int numberOfSteps) {
    cout << "\033[" << numberOfSteps << "C";
} // moveCursorRight.



// Cursor-Bewegung: Absolut.
// Hinweis: Es findet keine Ueberpruefung statt, ob die angegebenen Werte innerhalb der eigentlichen Terminalgroesse liegen.
void setCursorPosition(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
} // setCursorPosition.



// Terminal-Fenster komplett loeschen. Der Cursor springt im Anschluss auf die Position {1, 1} (= oben links).
void clearScreen() {
    cout << "\033[2J" << endl;
    setCursorPosition(1,1);
} // clearScreen.


// ##############################################################################
// #####                        KLASSE MENUE                                #####
// ##############################################################################


// Konstruktor mit Rahmenbegrenzung.
// In der Regel wird hier die Terminalgroesse uebergeben. Diese kann sich die Klasse nicht selber holen,
// da die Information in der Game-Engine gespeichert ist und dies eine Ebene ueber dieser Datei liegt.
Menu::Menu(int x_max, int y_max) {
    this->x_max = x_max;
    this->y_max = y_max;
    this->maxTextLength = 0;
    this->numberOfEntries = 0;
    this->currentPosition = 1;
    this->marker = ">";
} // Menu::Menu.


// Destruktor.
Menu::~Menu() {
    // Aktuell gibt es nichts zu tun.
} // Menu::~Menu.


// Hinzufuegen eines neuen Menue-Eintrags. Position gibt an, wo genau der Eintrag angefuegt werden soll.
void Menu::addEntry(string text, void (*menuFunction)(void), int position) {
    if ((position <= 0) || (position > this->numberOfEntries)) {
        // Am Ende einfuegen.
        this->vectorMenuText.push_back(text);
        this->vectorMenuFunctions.push_back(menuFunction);
    }
    else {
        // An gewuenschter Position einfuegen.
        this->vectorMenuText.insert(this->vectorMenuText.begin() + (position - 1), text);
        this->vectorMenuFunctions.insert(this->vectorMenuFunctions.begin() + (position - 1), menuFunction);
    }
    this->numberOfEntries++;

    // Hat der neu hinzugefuegte Menue-Eintrag einen laengeren Text als der bisher laengste Text?
    if (text.length() > this->maxTextLength) {
        this->maxTextLength = text.length();
        // Befindet sich der Text innerhalb der zulaessigen Laenge?
        if (this->maxTextLength > (this->x_max - 2)) {
            cerr << "Menue-Text zu lang!: " << text << endl;
        }
    }
} // Menu::addEntry.


// Loeschen eines Menue-Eintrags.
bool Menu::deleteEntry(int position) {
    // Sind ueberhaupt Eintrage zum Loeschen vorhanden?
    if (this->numberOfEntries == 0) {
        cout << "Keine Eintraege zum Loeschen vorhanden!" << endl;
        return false;
    }
    // Angabe gueltig?
    if (position > this->numberOfEntries) {
        cout << "Ungueltige Positionsangabe in Menu::deleteEntry. position > numberOfEntries!" << endl;
        return false;
    }
    else if (position < 0) {
        cout << "Ungueltige Positionsangabe in Menu::deleteEntry. position muss >= 0 sein!" << endl;
        return false;
    }
    else if (position == 0) {
        // Position = 0 gibt an, dass die letzte Position geloescht werden soll.
        position = this->numberOfEntries;
    }
    // Ueberpruefe, ob die maximale Textlaenge betroffen ist.
    if (this->vectorMenuText[position].length() == this->maxTextLength) {
        // Maximale Textlaenge muss neu berechnet werden.
        this->maxTextLength = 0;
        for (int i = 0; i < this->numberOfEntries; i++) {
            // Ueberspringe den gleich zu loeschenden Eintrag.
            if (i == (position - 1)) {
                continue;
            }
            // Suche unter allen anderen Texten nach dem Laengsten.
            if (this->vectorMenuText[i].length() > this->maxTextLength) {
                this->maxTextLength = this->vectorMenuText[i].length();
            }
        }
    }
    // Eintrag loeschen.
    this->vectorMenuText.erase(this->vectorMenuText.begin() + (position - 1));
    this->vectorMenuFunctions.erase(this->vectorMenuFunctions.begin() + (position - 1));
    this->numberOfEntries--;
    this->resetPosition();
    return true;
} // Menu::deleteEntry.


// Bewegt die Auswahl im Menue um eins nach oben.
void Menu::moveUp() {
    // Geht es noch hoeher?
    if (this->currentPosition > 1) {
        // Nach oben bewegen.
        this->refresh(currentPosition--);
    }
} // Menu::moveUp.


// Bewegt die Auswahl im Menue um eins nach unten.
void Menu::moveDown() {
    // Geht es noch niedriger?
    if (this->currentPosition < this->numberOfEntries) {
        // Nach unten bewegen.
        this->refresh(currentPosition++);
    }
} // Menu::moveDown.


// Setzt die Position wieder auf das erste Element.
void Menu::resetPosition() {
    this->currentPosition = 1;
} // Menu::resetPosition.


// Aktiviert die hinter der aktuellen Auswahl abgelegte Funktion.
void Menu::execute() {
    // Gibt es ueberhaupt einen Eintrag im Menue?
    if (this->numberOfEntries < 1) {
        cout << "Dieses Menue besitzt aktuell noch keine Eintraege!" << endl;
        return;
    }
    // Befindet sich angewaehlter Menuepunkt im Bereich der vorhandenen Menuepunkte?
    if (this->currentPosition > this->numberOfEntries) {
        cout << "Implementierungsfehler Menu::execute. currentPosition > numberOfEntries!" << endl;
        return;
    }
    this->vectorMenuFunctions[this->currentPosition - 1]();
} // Menu::execute.


// Visualisierung. Gibt das Menue am Bildschirm aus.
void Menu::draw() {
    // Bildschirm loeschen.
    clearScreen();

    // Nullposition ermitteln.
    int posX, posY;
    this->getMenuPosition(posX, posY);

    // Ausgabe des Menues.
    for (int i = 0; i < this->numberOfEntries; i++) {
        setCursorPosition(posX, posY++);
        if (i == (this->currentPosition - 1)) {
            // Aktuell angewaehlter Menuepunkt.
            cout << this->marker << " " << this->vectorMenuText[i];
        }
        else {
            // Aktuell nicht angewaehlter Menuepunkt.
            cout << "  " << this->vectorMenuText[i];
        }
    }
} // Menu::draw.


// Aktualisiert nur den Marker bei Aenderung der Auswahl im Menue.
void Menu::refresh(int positionOld) {
    // Nullposition des Menues ermitteln.
    int posX, posY;
    this->getMenuPosition(posX, posY);

    // Markierung an alter Position entfernen.
    setCursorPosition(posX, posY + (positionOld - 1));
    cout << " ";

    // Markierung an neuer Position setzen.
    setCursorPosition(posX, posY + (this->currentPosition - 1));
    cout << this->marker;

} // Menu::refresh.


// Individualisierung. Aendert das Marker-Zeichen.
bool Menu::setMarker(string marker) {
    // Marker darf nur ein Zeichen lang sein.
    if (marker.length() != 1) {
        cout << "Der Marker " << marker << " ist fuer das Menue ungueltig!" << endl;
        return false;
    }
    this->marker = marker; 
    
    // Ein Neuzeichnen findet nicht statt, da nicht gesagt ist, dass das Menue
    // ueberhaupt aktuell angezeigt wird. 
    // Ggf. ist ein manuelles Neuzeichnen erforderlich.

    return true;
} // Menu::setMarker.


// Ermittlung der Zeichen-Nullposition.
void Menu::getMenuPosition(int &x, int &y) {
    x = ((this->x_max - this->maxTextLength) / 2) - 5;
    y = ((this->y_max - this->numberOfEntries) / 2) - 1;
} // Menu::getMenuePosition.
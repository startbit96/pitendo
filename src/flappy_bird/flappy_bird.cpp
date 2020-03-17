#include <iostream>
#include <algorithm>        // min, max.
#include <stdlib.h>         // rand.
#include <tgmath.h>         // sqrt. 

#include "pitendo_game_engine.h"
#include "flappy_bird.h"


using namespace std;
using namespace flappyBird;


// ##############################################################################
// #####                    SPIELOBJEKTE DEFINIEREN                         #####
// ##############################################################################

// Unser Spielfeld.
PlayingField *playground;

// Unser Flappy Bird.
Bird *bird;

// Game-Over-Funktion ankuendigen.
void gameOverScreen();

// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Diese Funktion wird einmalig aufgerufen, wenn im Hauptmenue das Spiel
// angewaehlt wird. 
void flappyBird::gameStart() {
    // Hauptmenue korrekt beenden.
    mainMenu::menuExit();

    // Options-Menue an das Spiel anpassen.
    if (pitendoGE->customiseOptionMenu(&flappyBird::gameReturn, &flappyBird::gameStop) == false) {
        return;
    }


    // Spiel-Bildschirm darstellen.
    clearScreen();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Objekte erstellen.
    playground = new PlayingField();
    bird = new Bird();

    // Zufallszahlengenerator initialisieren. 
    // Abhaengig von Startzeit der WiringPi-Initialisierung.
    srand(micros());

    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunction = &flappyBird::gameHandle;

} // flappyBird::gameStart.


// Diese Funktion wird bei laufendem Spiel in jedem Schleifendurchlauf aufgerufen.
void flappyBird::gameHandle() {
    // Soll pausiert werden?
    if (controllerP1->buttonStart->wasPressed() || controllerP2->buttonStart->wasPressed()) {
        flappyBird::gamePause();
        return;
    }

    // Soll der Vogel nach oben fliegen?
    if (controllerP1->buttonGruen->wasPressed() || controllerP2->buttonGruen->wasPressed()) {
        bird->fly();
    }

    // Ueberpruefe, ob Bewegung stattfinden darf (FPS einstellen).
    if (++playground->timeNow >= DEF_TIME_PER_MOVEMENT) {
        // Ja, Bewegung darf stattfinden.

        // Rohre bewegen.
        playground->moveLeft();
        playground->timeNow = 0;

        // Ueberpruefe, ob ein neues Rohr erzeugt werden kann (Abstand ausreichend?).
        if (playground->numberOfPipes == 0) {
            // Es gibt noch gar kein Rohr. Erzeuge das erste.
            playground->addPipe((int)((pitendoGE->screenHeight - DEF_PIPE_GATE_HEIGHT) / 2)); 
        }
        else {
            // Es gibt bereits Rohre. Das Rohr, welches am naechsten zum rechten Spielfeldrand
            // ist, befindet sich im letzten Eintrag des Vektors.
            if ((pitendoGE->screenWidth - playground->vectorPipes.back().posX - DEF_PIPE_WIDTH) >= DEF_PIPE_DISTANCE) {
                    // Erzeuge ein neues Rohr.
                    // Das Rohr soll sich auf einer zufaelligen Hoehe in dem gegeben zulaessigen Bereich befinden.
                    // Dieser Bereich ist abhaengig von der Position des letzten Durchgangs des letzten Rohres.
                    // Ermittle Position des letzten Rohres.
                    int posYPast = playground->vectorPipes.back().posY;
                    // Minimalen Abstand zur oberen Wand wahren.
                    int posYMin = max(  posYPast - DEF_PIPE_GATE_Y_GATE_DISTANCE, 
                                        DEF_PIPE_GATE_Y_WALL_DISTANCE);
                    // Minimalen Abstand zur unteren Wand wahren.
                    int posYMax = min(  posYPast + DEF_PIPE_GATE_Y_GATE_DISTANCE, 
                                        pitendoGE->screenHeight - DEF_PIPE_GATE_HEIGHT - DEF_PIPE_GATE_Y_WALL_DISTANCE);
                    // In dem ermittelten zulaessigen Bereich soll nun ein zufaelliger Wert ausgewaehlt werden.
                    // Der zufaellige Wert soll jedoch mit einer hoeheren Wahrscheinlichkeit moeglichst weit
                    // vom aktuellen Wert entfernt liegen.
                    // Deswegen werden die zulaessigen Werte quadriert, nullpunktverschoben, ein Zufallswert ermittelt und im Anschluss
                    // wieder entgegen Null verschoben und die Wurzel gebildet. Damit sind grosse Distanzen wahrscheinlicher als kleine Distanzen.
                    int randomScope =   ((posYPast - posYMin) * (posYPast - posYMin)) + 
                                        ((posYMax - posYPast) * (posYMax - posYPast));
                    int randomNumber = (rand() % randomScope) - ((posYPast - posYMin) * (posYPast - posYMin));
                    int posYNew = posYPast;
                    if (randomNumber < 0) {
                        // Neuer Rohrdurchgang liegt ueberhalb altem Rohrdurchgang.
                        posYNew = posYNew - (int)sqrt((-1.0f) * randomNumber);
                    }
                    else {
                        // Neuer Rohrdurchgang liegt unterhalb altem Rohrdurchgang.
                        posYNew = posYNew + (int)sqrt(randomNumber);
                    }
                    // Neues Rohr erstellen.
                    playground->addPipe(posYNew);   
                }
        }

        // Vogel an alter Position loeschen.
        bird->remove();
        // Wohin soll der Vogel fliegen?
        bird->calculatePosition();
        // Vogel zeichnen.
        bird->draw();

        // Ueberpruefe auf Kollision.
        int posX1, posX2, posY1, posY2;
        // Vogel-Position ermitteln.
        bird->getPosition(posX1, posX2, posY1, posY2);
        if (playground->checkForCollision(posX1, posX2, posY1, posY2) == true) {
            // Kollision erkannt.
            cout << flush;
            // Spiel fuer eine Sekunde pausieren, damit Nutzer Fehler erkennt.
            delay(1000);
            // Game-Over-Bildschirm anzeigen.
            gameOverScreen();
        }
    }

} // flappyBird::gameHandle.


// Diese Funktion wird aufgerufen, wenn im Spiel "Pause" gedrueckt wird. 
void flappyBird::gamePause() {
    // Zwischenstand abspeichern?
    // ... in diesem Fall nicht.


    // Options-Menue anzeigen.
    optionMenu::menuStart();

} // flappyBird::gamePause.


// Diese Funktion wird aufgerufen, wenn aus dem Pausenmenue zurueck zum Spiel
// gekehrt wird.
void flappyBird::gameReturn() {
    // Option-Menue korrekt beenden.
    optionMenu::menuExit();

    // Spiel-Bildschirm darstellen.
    clearScreen();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Pitendo-Spielfunktion (fuer Dauerschleife) setzen.
    pitendoGE->gameEngineFunction = &flappyBird::gameHandle;

} // flappyBird::gameReturn.


// Diese Funktion wird aufgerufen, wenn das Spiel beendet wird und zurueck zum
// Hauptmenue gekehrt wird. Es koennen final Spielstaende gesichert werden.
void flappyBird::gameStop() {
    // Muessen Spielstaende gespeichert werden?
    // ... in diesem Fall nicht. 

    // Muss Speicher wieder freigegeben werden?
    // ... in diesem Fall nicht. 

    // Verwerfe im Optionen-Menue die spielabhaengigen Eintraege.
    if (pitendoGE->resetOptionMenu() == false) {
        return;
    }

    // Rufe wieder das Hauptmenue auf. Das darf nicht entfernt werden,
    // da sonst aus dem Spiel nicht mehr ins Hauptmenue zurueckgekehrt werden kann.
    mainMenu::menuStart();

} // flappyBird::gameStop.



// ##############################################################################
// #####                    INDIVIDUELLE FUNKTIONEN                         #####
// ##############################################################################


// ##############################################################################
// #####                      KLASSE PLAYINGFIELD                           #####
// ##############################################################################


// Konstruktor Klasse PlayingField.
PlayingField::PlayingField() {
    // Anzahl von Rohren.
    this->numberOfPipes = 0;

    // Timing.
    this->timeNow = 0;
} // Konstruktor Klasse PlayingField.


// Destruktor Klasse PlayingField.
PlayingField::~PlayingField() {

} // Destruktor Klasse PlayingField.


// Funktion zum Hinzufuegen eines neuen Rohres.
void PlayingField::addPipe(int posY) {
    this->vectorPipes.push_back(Pipe(posY));
    this->numberOfPipes++;
} // PlayingField::addPipe.


// Funktion zum Zeichnen des Spielfeldes am Bildschirm.
void PlayingField::draw() {
    // Zeichne jedes Rohr.
    for (int i = 0; i < this->numberOfPipes; i++) {
        vectorPipes[i].draw();
    }
} // PlayingField::draw.


// Funktion zum Weiterbewegen des Spielfeldes.
void PlayingField::moveLeft() {
    // Schiebe alles nach links. Ggf. muss ein Rohr geloescht werden.
    // Hinweis: Das Programm geht in die Knie, wenn mehrere Rohre gleichzeitig das
    // Ende erreichen. Das ist zu verhindern, da hier keine Fehlerbehandlung stattfindet.
    int posDelete = -1;
    for (int i = 0; i < this->numberOfPipes; i++) {
        if (this->vectorPipes[i].moveLeft() == false) {
            // Rohr ist am linken Bildschirm-Rand angekommen und wurde bereits nicht
            // mehr gezeichnet.
            posDelete = i;

            // Speicherfreigabe mittels delete hat zu Systemabsturz gefuehrt ...
        }
    }
    // Handle loeschen.
    if (posDelete != -1) {
        this->vectorPipes.erase(this->vectorPipes.begin() + posDelete);
        this->numberOfPipes--;
    }
} // PlayingField::moveLeft.


// Funktion zum Checken, ob eine Kollision vorliegt.
bool PlayingField::checkForCollision(int posX1, int posX2, int posY1, int posY2) {
    for (int i = 0; i < this->numberOfPipes; i++) {
        if (this->vectorPipes[i].checkForCollision(posX1, posX2, posY1, posY2) == true) {
            // Kollision festgestellt.
            return true;
        }
    }
    // Wenn du hier ankommst, wurde innerhalb der Schleife noch keine Kollision festgestellt.
    // Dann liegt auch keine vor.
    return false;
} // PlayingField::checkForCollision.


// ##############################################################################
// #####                         KLASSE PIPE                                #####
// ##############################################################################


// Konstruktor Klasse Pipe.
PlayingField::Pipe::Pipe(int posY) {
    // Position der Rohre in x-Richtung. Diese Angabe bezieht sich auf
    // den am weitesten links liegenden Punkt der Rohre.
    this->posX = pitendoGE->screenWidth - 8;

    // Position des Durchganges in y-Richtung. Diese Angabe bezieht sich auf
    // das Ende des oberen Rohres.
    this->posY = posY;
} // Konstruktor Klasse Pipe.


// Destruktor Klasse Pipe.
PlayingField::Pipe::~Pipe() {

} // Destruktor Klasse Pipe.


// Funktion zum Zeichnen der beiden Rohre am Bildschirm.
// (inkl. Loeschen der alten gemalten Position)
void PlayingField::Pipe::draw() {
    for (int posY = 1; posY <= pitendoGE->screenHeight; posY++) {
        if ((posY == this->posY) || (posY == (this->posY + DEF_PIPE_GATE_HEIGHT + 1))) {
            // Rohr-Enden.
            setCursorPosition(this->posX, posY);
            cout << "[";
            for (int i = 0; i < DEF_PIPE_WIDTH - 2; i++) cout << "=";
            cout << "] ";   // Mit Leerzeichen am Ende, um vorherige Position loeschen zu koennen.
        }
        else if ((posY < this->posY) || (posY > (this->posY + DEF_PIPE_GATE_HEIGHT + 1))) {
            // Rohr.
            setCursorPosition(this->posX, posY);
            cout << "|";
            for (int i = 0; i < DEF_PIPE_WIDTH - 2; i++) cout << " ";
            cout << "| ";   // Mit Leerzeichen am Ende, um vorherige Position loeschen zu koennen.
        }
    }
} // PlayingField::Pipe::draw.


// Funktion zum Loeschen des Rohres.
void PlayingField::Pipe::remove() {
    for (int posY = 1; posY <= pitendoGE->screenHeight; posY++) {
        setCursorPosition(this->posX, posY);
        for (int i = 0; i < DEF_PIPE_WIDTH; i++) cout << " "; 
    }
} // PlayingField::Pipe::remove.


// Funktion zum Checken, ob eine Kollision vorliegt.
bool PlayingField::Pipe::checkForCollision(int posX1, int posX2, int posY1, int posY2) {
    // Ueberpruefe Uebereinstimmung in x-Richtung.
    if ((posX1 <= (this->posX + DEF_PIPE_WIDTH - 1)) && (posX2 >= this->posX)) {
        // x-Bereich ueberschneidet sich.
        // Ueberpruefe Uebereinstimmmung in y-Richtung.
        if ((posY1 > this->posY) && (posY2 < (this->posY + DEF_PIPE_GATE_HEIGHT))) {
            // Befindet sich innerhalb des Rohr-Tores.
            // Keine Kollision.
            return false;
        }
        else {
            // Befindet sich im Bereich der Rohre.
            // Kollision.
            return true;
        }
    }
    else {
        // x-Bereich ueberschneidet sich nicht.
        // Keine Kollision.
        return false;
    }
} // PlayingField::Pipe::checkForCollision.


// Weiterbewegung des Rohres (inkl. Loeschen der alten Position).
// Der Rueckgabewert ist true, wenn weiter geschoben werden
// konnte und ist false, wenn das Ende des Bildschirms erreicht ist
// und somit das Rohr geloescht werden kann.
// Im Falle des Erreichens des Randes des Bildschirms, loescht sich das
// Rohr selber von dem Bildschirm.
bool PlayingField::Pipe::moveLeft() {
    if (this->posX == 1) {
        // Spielfeldrand erreicht. Lebensdauer des Rohres ist erreicht.
        this->remove();
        return false;
    }
    else {
        // Rohr kann noch weiter nach links verschoben werden.
        this->posX--;
        this->draw();
        return true;
    }
} // PlayingField::Pipe::moveLeft.


// ##############################################################################
// #####                         KLASSE BIRD                                #####
// ##############################################################################


// Konstruktor Klasse Bird.
Bird::Bird() {
    // Positionierung.
    this->posX = (int)(pitendoGE->screenWidth / 2);
    this->posY = (int)(pitendoGE->screenHeight / 2);

    // Geschwindigkeit.
    this->speed = 0.0f;
} // Konstruktor Bird::Bird.


// Destruktor Klasse Bird.
Bird::~Bird() {

} // Destruktor Bird::Bird.


// Funktion zum Zeichnen des Vogels am Bildschirm.
void Bird::draw() {
    /*
    Variante 1:

            _==_
        ^__/(00)>
        (\/////)
         ~~~**~
    
    Variante 2:

         _==_
        /(00)>
        (~"")

    Bei Aenderung muss in der .h-Datei die Breite und Hoehe des Vogels in den
    Defines angepasst werden.
    */

    setCursorPosition(this->posX, this->posY);
    cout << "    _==_ ";
    setCursorPosition(this->posX, this->posY + 1);
    cout << "^__/(00)>";
    setCursorPosition(this->posX, this->posY + 2);
    cout << "(\\/////) ";
    setCursorPosition(this->posX, this->posY + 3);
    cout << " ~~~**~  ";
} // Bird::draw.


// Funktion zum Entfernen des Vogels am Bildschirm.
void Bird::remove() {
    for (int y = 0; y < DEF_BIRD_HEIGHT; y++) {
        setCursorPosition(this->posX, this->posY + y);
        for (int x = 0; x < DEF_BIRD_WIDTH; x++) cout << " ";
    }
} // Bird::remove.


// Funktion zum Aufsteigen / nach oben fliegen.
void Bird::fly() {
    this->speed = this->speed + (float)DEF_BIRD_ACC_FLYING;
} // Bird::fly.


// Funktion zur Neuberechnung der naechsten Position.
// Ist abhaengig von der Geschwindigkeit in y-Richtung.
// Einfluss hierauf nehmen die fest definierte Schwerkraft sowie die Nutzer-
// Eingaben abhaengige Beschleunigung durch das Fliegen.
void Bird::calculatePosition() {
    // Einberechnung der Gewichtskraft (Beschleunigung im Fallen).
    this->speed = this->speed + (float)DEF_BIRD_ACC_FALLING;
    // Geschwindigkeit im zulaessigen Bereich?
    if (this->speed < ((-1) * DEF_BIRD_MAX_SPEED)) this->speed = (-1) * DEF_BIRD_MAX_SPEED;
    else if (this->speed > DEF_BIRD_MAX_SPEED) this->speed = DEF_BIRD_MAX_SPEED;

    // Berechnung der Weg-Aenderung.
    this->posY = this->posY + (int)this->speed;
    // Im zulaessigen Bereich?
    if (this->posY < 1) {
        // Vogel befindet sich an der Decke.
        this->posY = 1;
        // Setze ausserdem die Geschwindigkeit auf Null, um Chance zu bieten, schnell
        // wieder von den Grenzwerten wegzukommen.
        this->speed = 0.0f;
    }
    else if (this->posY > (pitendoGE->screenHeight - DEF_BIRD_HEIGHT + 1)) {
        // Vogel befindet sich am Boden.
        this->posY = pitendoGE->screenHeight - DEF_BIRD_HEIGHT + 1;
        // Setze ausserdem die Geschwindigkeit auf Null, um Chance zu bieten, schnell
        // wieder von den Grenzwerten wegzukommen.
        this->speed = 0.0f;        
    }
} // Bird::calculatePosition.


// Position des Vogels erhalten.
void Bird::getPosition(int &posX1, int &posX2, int &posY1, int &posY2) {
    posX1 = this->posX;
    posX2 = this->posX + DEF_BIRD_WIDTH - 1;
    posY1 = this->posY;
    posY2 = this->posY + DEF_BIRD_HEIGHT - 1;
} // Bird::getPosition.


// ##############################################################################
// #####                           SONSTIGES                                #####
// ##############################################################################

void gameOverScreen() {
    // Bildschirm loeschen.
    clearScreen();

    // Game-Over anzeigen.
    int posX = (int)((pitendoGE->screenWidth - 90) / 2);
    int posY = (int)((pitendoGE->screenHeight - 8) / 2);
    setCursorPosition(posX, posY++);
    cout << " ######      ###    ##     ## ########     #######  ##     ## ######## ########";
    setCursorPosition(posX, posY++);
    cout << "##    ##    ## ##   ###   ### ##          ##     ## ##     ## ##       ##     ##";
    setCursorPosition(posX, posY++);
    cout << "##         ##   ##  #### #### ##          ##     ## ##     ## ##       ##     ##";
    setCursorPosition(posX, posY++);
    cout << "##   #### ##     ## ## ### ## ######      ##     ## ##     ## ######   ########";
    setCursorPosition(posX, posY++);
    cout << "##    ##  ######### ##     ## ##          ##     ##  ##   ##  ##       ##   ##";
    setCursorPosition(posX, posY++);
    cout << "##    ##  ##     ## ##     ## ##          ##     ##   ## ##   ##       ##    ##";
    setCursorPosition(posX, posY++);
    cout << " ######   ##     ## ##     ## ########     #######     ###    ######## ##     ##";

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Buttons neu belegen (mit jedem wird das Spiel beendet).
    controllerP1->setButtonFunctions(   &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop);
    controllerP2->setButtonFunctions(   &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop,
                                        &flappyBird::gameStop);

    // Game-Engine-Funktion belegen.
    pitendoGE->gameEngineFunction = &controllerExecute;
}
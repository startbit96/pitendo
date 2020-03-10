#include "pitendo_game_engine.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

// Unsere Game-Engine fuer Pitendo.
GameEngine* pitendoGE;


// ##############################################################################
// #####                        GRUNDFUNKTIONEN                             #####
// ##############################################################################


// Initialisierung des Systems.
// Diese Funktion prueft das Vorhandensein der notwendigen Ressourcen (xterm, Schriftart).
// Wenn alles passt, wird das Programm in einem separaten Fenster erneut aufgerufen,
// um die Einstellungen (Vollbild, ...) setzen zu koennen.
bool pitendoInit() {
    // Informiere Nutzer.
    cout << "Ueberpruefe Abhaengigkeiten ..." << endl;

    // Wird ueber Remote-Development programmiert (z.B. Visual Studio Code mit SSH), muss 
    // durch das Setzen einer Systemvariable erst angegeben werden, welcher Display genutzt werden
    // soll. Wir moechten natuerlich den des Raspberry Pi.
    setenv("DISPLAY", ":0", 1);

    // Ist auf dem Raspberry Pi xterm installiert? Dies ist der in dem Projekt Pitendo verwendete Terminal-Emulator.
    if (FILE *file = fopen("/usr/bin/xterm", "r")) {
        // i.O.
        fclose(file);
        cout << "Terminal-Emulator xterm ist installiert." << endl;
    }
    else {
        // n.i.O.
        cout << "Auf dem Raspberry Pi ist kein xterm installiert." << endl;
        cout << "Installiere erst mittels \"sudo apt-get install xterm\" den Terminal-Emulator." << endl;
        return false;
    }

    // Fuer die grafische Darstellung im Terminal eignen sich besonders quadratische Schriftarten gut.
    // Quadratisch bedeutet, dass Zeichen gleich hoch wie breit sind.
    // Pruefe daher, ob die Schrift "square" installiert ist.
    if (FILE *file = fopen("/home/pi/.fonts/square.ttf", "r")) {
        // i.O.
        fclose(file);
        cout << "Schriftart square gefunden." << endl;
    }
    else {
        // n.i.O.
        cout << "Auf dem Raspberry Pi ist kein square installiert." << endl;
        cout << "Siehe README.md fuer mehr Informationen zur Installation." << endl;
        return false;
    }

    // Neues Terminal oeffnen und Pitendo mit hoher Prozess-Prioritaet ausfuehren.
    const int pitendoProzessPrioritaet = -20;
    const int fontSize = 6;
    //string consoleCommand = "sudo nice -n " + to_string(pitendoProzessPrioritaet); // Prozess-Prioritaet.
    string consoleCommand = "";
    consoleCommand += "xterm -fa \"square:size=" + to_string(fontSize) + ":antialias=true\""; // Schriftart.
    consoleCommand += " -fullscreen -e \"./build/pitendo start\"";
    cout << "Ausfuehren von \"" << consoleCommand << "\"" << "..." << endl;
    // Konsolen-Kommando ausfuehren.
    system(consoleCommand.c_str());
    return true;
} // pitendoInit;


// Einrichtung der Pitendo-Umgebung.
// Unterschied zu Initialisierung: Initialisierung prueft Abhaengigkeiten. Setup richtet ein.
bool pitendoSetup() {
    // Cursor verstecken.
    system("setterm -cursor off");

    // Controller initialisieren.
    if (controllerSetup() == true) {
        cout << "Controller erfolgreich initalisiert." << endl;
    }
    else {
        cout << "Controller konnte nicht initialisiert werden." << endl;
        return false;
    }

    // System kurz zur Ruhe kommen lassen, da sonst aus irgendwelchen Gruenden durch die 
    // Fullscreen-Einstellung die tatsaechliche Terminalgroesse dem System noch nicht bekannt ist.
    delay(100);

    // Terminal-Groesse ermitteln.
    struct winsize windowSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);
    cout << "Fenstergroesse: " << windowSize.ws_row << "x" << windowSize.ws_col << endl;

    // Pitendo-Game-Engine erstellen.
    pitendoGE = new GameEngine(windowSize.ws_col, windowSize.ws_row);

    return true;
} // pitendoSetup.


// Start-Bildschirm.
void pitendoStartScreen() {
    // Display komplett bereinigen.
    clearScreen();
    // "Pitendo" in ASCII-Art ausgeben.
    int posX = (pitendoGE->screenWidth - 185) / 2;
    int posY = (pitendoGE->screenHeight - 30) / 2;
    #pragma region // textStartScreen
    setCursorPosition(posX, posY++);
    cout << "          _____                    _____                _____                    _____                    _____                    _____                   _______         " << endl;
    setCursorPosition(posX, posY++);
    cout << "         /\\    \\                  /\\    \\              /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                 /::\\    \\        " << endl;
    setCursorPosition(posX, posY++);
    cout << "        /::\\    \\                /::\\    \\            /::\\    \\                /::\\    \\                /::\\____\\                /::\\    \\               /::::\\    \\       " << endl;
    setCursorPosition(posX, posY++);
    cout << "       /::::\\    \\               \\:::\\    \\           \\:::\\    \\              /::::\\    \\              /::::|   |               /::::\\    \\             /::::::\\    \\      " << endl;
    setCursorPosition(posX, posY++);
    cout << "      /::::::\\    \\               \\:::\\    \\           \\:::\\    \\            /::::::\\    \\            /:::::|   |              /::::::\\    \\           /::::::::\\    \\     " << endl;
    setCursorPosition(posX, posY++);
    cout << "     /:::/\\:::\\    \\               \\:::\\    \\           \\:::\\    \\          /:::/\\:::\\    \\          /::::::|   |             /:::/\\:::\\    \\         /:::/~~\\:::\\    \\    " << endl;
    setCursorPosition(posX, posY++);
    cout << "    /:::/__\\:::\\    \\               \\:::\\    \\           \\:::\\    \\        /:::/__\\:::\\    \\        /:::/|::|   |            /:::/  \\:::\\    \\       /:::/    \\:::\\    \\   " << endl;
    setCursorPosition(posX, posY++);
    cout << "   /::::\\   \\:::\\    \\              /::::\\    \\          /::::\\    \\      /::::\\   \\:::\\    \\      /:::/ |::|   |           /:::/    \\:::\\    \\     /:::/    / \\:::\\    \\  " << endl;
    setCursorPosition(posX, posY++);
    cout << "  /::::::\\   \\:::\\    \\    ____    /::::::\\    \\        /::::::\\    \\    /::::::\\   \\:::\\    \\    /:::/  |::|   | _____    /:::/    / \\:::\\    \\   /:::/____/   \\:::\\____\\ " << endl;
    setCursorPosition(posX, posY++);
    cout << " /:::/\\:::\\   \\:::\\____\\  /\\   \\  /:::/\\:::\\    \\      /:::/\\:::\\    \\  /:::/\\:::\\   \\:::\\    \\  /:::/   |::|   |/\\    \\  /:::/    /   \\:::\\ ___\\ |:::|    |     |:::|    |" << endl;
    setCursorPosition(posX, posY++);
    cout << "/:::/  \\:::\\   \\:::|    |/::\\   \\/:::/  \\:::\\____\\    /:::/  \\:::\\____\\/:::/__\\:::\\   \\:::\\____\\/:: /    |::|   /::\\____\\/:::/____/     \\:::|    ||:::|____|     |:::|    |" << endl;
    setCursorPosition(posX, posY++);
    cout << "\\::/    \\:::\\  /:::|____|\\:::\\  /:::/    \\::/    /   /:::/    \\::/    /\\:::\\   \\:::\\   \\::/    /\\::/    /|::|  /:::/    /\\:::\\    \\     /:::|____| \\:::\\    \\   /:::/    / " << endl;
    setCursorPosition(posX, posY++);
    cout << " \\/_____/\\:::\\/:::/    /  \\:::\\/:::/    / \\/____/   /:::/    / \\/____/  \\:::\\   \\:::\\   \\/____/  \\/____/ |::| /:::/    /  \\:::\\    \\   /:::/    /   \\:::\\    \\ /:::/    /  " << endl;
    setCursorPosition(posX, posY++);
    cout << "          \\::::::/    /    \\::::::/    /           /:::/    /            \\:::\\   \\:::\\    \\              |::|/:::/    /    \\:::\\    \\ /:::/    /     \\:::\\    /:::/    /   " << endl;
    setCursorPosition(posX, posY++);
    cout << "           \\::::/    /      \\::::/____/           /:::/    /              \\:::\\   \\:::\\____\\             |::::::/    /      \\:::\\    /:::/    /       \\:::\\__/:::/    /    " << endl;
    setCursorPosition(posX, posY++);
    cout << "            \\::/____/        \\:::\\    \\           \\::/    /                \\:::\\   \\::/    /             |:::::/    /        \\:::\\  /:::/    /         \\::::::::/    /     " << endl;
    setCursorPosition(posX, posY++);
    cout << "             ~~               \\:::\\    \\           \\/____/                  \\:::\\   \\/____/              |::::/    /          \\:::\\/:::/    /           \\::::::/    /      " << endl;
    setCursorPosition(posX, posY++);
    cout << "                               \\:::\\    \\                                    \\:::\\    \\                  /:::/    /            \\::::::/    /             \\::::/    /       " << endl;
    setCursorPosition(posX, posY++);
    cout << "                                \\:::\\____\\                                    \\:::\\____\\                /:::/    /              \\::::/    /               \\::/____/        " << endl;
    setCursorPosition(posX, posY++);
    cout << "                                 \\::/    /                                     \\::/    /                \\::/    /                \\::/____/                 ~~              " << endl;
    setCursorPosition(posX, posY++);
    cout << "                                  \\/____/                                       \\/____/                  \\/____/                  ~~                                       " << endl;
    setCursorPosition(posX, posY + 5);
    cout << "                                                                    >> GRUEN DRUECKEN UM MIT DEM SPIELEN ZU BEGINNEN <<                                                     " << endl;
    #pragma endregion

    // Button-Funktionen definieren.
    // Nach Druecken des Gruenen Buttons wird das Hauptmenue aufgerufen.
    controllerP1->setButtonFunctions(   &pitendoStart,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion);
    controllerP2->setButtonFunctions(   &pitendoStart,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion,
                                        &Button::defaultButtonFunktion);

    // GameEngine aktualisieren.
    pitendoGE->gameEngineFunktion = &controllerExecute;

} // pitendoStartScreen.


// Uebergang vom Start-Bildschirm zum Hauptmenue.
// Der Unterschied zum normalen Aufruf des Hauptmenues besteht darin, ggf. noch vorherige Aktivitaeten des 
// Startbildschirms zu beenden (z.B. Startbildschirm-Musik).
void pitendoStart() {
    // Muss noch etwas vom Startmenue terminiert werden?
    // ... aktuell nicht.

    // Hauptmenue aurufen.
    mainMenu::menuStart();

} // pitendoStart.


// Beendet Pitendo.
void pitendoExit() {
    pitendoGE->isRunning = false;
} // pitendoExit.


// Loest alle Abhaengigkeiten auf, damit das Programm sauber beendet werden kann.
void pitendoDestroy() {
    // Aktuell nichts zu tun. Alles was getan werden muss, ist in den Destruktoren der
    // verschiedenen Klassen implementiert und diese werden automatisch aufgerufen.
}



// ##############################################################################
// #####                       KLASSE GAME ENGINE                           #####
// ##############################################################################


// Konstruktor.
GameEngine::GameEngine(int screenWidth, int screenHeight) {
    this->isRunning = true;
    this->gameEngineFunktion = &defaultGameEngineFunction;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->pitendoState = PITENDO_WAITING;
    this->fps = DEF_DEFAULT_FPS;
    this->loopTimeOld = 0;

    // Haupt-Menue definieren.
    this->mainMenu = new Menu(screenWidth, screenHeight);
    this->mainMenu->addEntry("Optionen", &optionMenu::menuStart);
    this->mainMenu->addEntry("Pitendo beenden", &pitendoExit);

    // Optionen-Menue definieren.
    this->optionMenu = new Menu(this->screenWidth, this->screenHeight);
    this->optionMenu->addEntry("Hauptmenue", &mainMenu::menuStart);

} // GameEngine::GameEngine.


// Destruktor.
GameEngine::~GameEngine() {

} // GameEngine::~GameEngine.


// Zeitliches Timing in der Dauerschleife.
// Linux ist kein Echtzeit-Betriebssystem, daher sind die FPS eher als ueber 
// den Daumen gepeilt anzusehen.
// FPS setzen.
bool GameEngine::setFPS(int fps) {
    if ((fps > 0) && (fps <= 100)) {
        this->fps = fps;
        return true;
    }
    else {
        return false;
    }
} // GameEngine::setFPS.


// FPS einstellen.
void GameEngine::adjustFPS() {
    // System pausieren.
    unsigned int loopTimeNew = millis();
    // Sichergehen, dass Werte im zulaessigen Bereich sind und die unsigned int Grenze
    // nicht ueberlaufen wurde.
    if (loopTimeNew > this->loopTimeOld) {
        // Muss ueberhaupt noch gewartet werden oder hinken wir eh bereits hinterher?
        int delayTime = (int)(1000 / this->fps) - (loopTimeNew - this->loopTimeOld);
        if (delayTime > 0) {
            delay(delayTime);
        }
    }
    // "Neue alte" Zeit speichern.
    this->loopTimeOld = millis();
} // GameEngine::adjustFPS.


// Hinzufuegen eines neuen Spieles mit Uebergabe der Spielstart-Funktion.
void GameEngine::addGame(std::string gameName, void (*gameStartFunction)(void)) {
    this->mainMenu->addEntry(gameName, gameStartFunction, 1);
} // GameEngine::addGame.


// Wenn ein Spiel gestartet wurde, ist auch das Optionen-Menue an die spieleigene
// Pausenfunktion und Beendenfunktion anzupassen.
bool GameEngine::customiseOptionMenu(void (*gameReturnFunction)(void), void (*gameStopFunction)(void)) {
    // Ueberpruefe Pitendo-State, ob Anpassung erfolgen darf.
    if (this->pitendoState != PITENDO_WAITING) {
        // Ein Fehler liegt vor!
        // Hinweis: Menue wurde wohl an anderer Stelle bereits angepasst oder zuvor nicht resettet.
        cout << "Fehler in Implementierung der Options-Menue-Anpassung." << endl;
        cout << "Option-Menue wurde seit der letzten Menue-Anpassung nicht mehr resettet." << endl;
        return false;
    }
    // Eintrag "Weiterspielen" anlegen.
    this->optionMenu->addEntry("Weiterspielen", gameReturnFunction, 1);
    // Eintrag "Hauptmenue" loeschen und mit spielindividueller Menue-Aufruf-Funktion neu ablegen.
    this->optionMenu->deleteEntry(0);
    this->optionMenu->addEntry("Hauptmenue", gameStopFunction);
    // Pitendo-State aktualisieren.
    this->pitendoState = PITENDO_RUNNING;
    return true;
} // GameEngine::customiseOptionMenu.


// Wenn das Spiel verlassen wurde, ist die eben angesprochene Anpassung des
// Optionenmenues wieder rueckgaengig zu machen.
bool GameEngine::resetOptionMenu() {
    // Ueberpruefe Pitendo-State, ob Reset erfolgen darf.
    if (this->pitendoState != PITENDO_RUNNING) {
        // Ein Fehler liegt vor!
        // Hinweis: Menue wurde wohl an anderer Stelle bereits resettet oder gar nicht erst angepasst?
        cout << "Fehler in Implementierung der Options-Menue-Anpassung. (Reset)" << endl;
        cout << "Options-Menue soll resettet werden, wurde aber bislang gar nicht angepasst." << endl;
        return false;
    }
    // Eintrag "Weiterspielen" loeschen.
    this->optionMenu->deleteEntry(1);
    // Eintrag "Hauptmenue" loeschen und mit normaler Menue-Aufruf-Funktion neu ablegen.
    this->optionMenu->deleteEntry(0);
    this->optionMenu->addEntry("Hauptmenue", &mainMenu::menuStart);
    // Pitendo-State aktualisieren.
    this->pitendoState = PITENDO_WAITING;
    return true;
} // GameEngine::resetOptionMenu.



// Funktion fuer die Initialisierung.
void GameEngine::defaultGameEngineFunction() {
    // Mache einfach nix.
} // GameEngine::defaultGameEngineFunction.



// ##############################################################################
// #####                        PITENDO-MENUES                              #####
// ##############################################################################


// Einmaliger Aufruf, welcher intern die Button-Handler auf menueHandler abgibt.
void mainMenu::menuStart() {
    // Menue darstellen.
    pitendoGE->mainMenu->resetPosition();
    pitendoGE->mainMenu->draw();

    // Klasseneigenen Button-Handler deaktivieren.
    // Wird mittels menuHandler geregelt.
    controllerDeactivate();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Neuen Handler setzen.
    pitendoGE->gameEngineFunktion = &mainMenu::menuHandler;

} // mainMenu::menuStart.


// Dauerhafter Button-Handler.
void mainMenu::menuHandler() {
    // Joystick-Bewegung auslesen.
    bool upC1, downC1, leftC1, rightC1;
    bool upC2, downC2, leftC2, rightC2;
    controllerP1->joystick->getMovement(upC1, downC1, leftC1, rightC1);
    controllerP2->joystick->getMovement(upC2, downC2, leftC2, rightC2);

    // Soll im Menue nach oben oder unten gewandert werden?
    if (upC1 || upC2) {
        pitendoGE->mainMenu->moveUp();
    }
    if (downC1 || downC2) {
        pitendoGE->mainMenu->moveDown();
    }

    // Wurde ein Menue-Punkt angewaehlt?
    if ((controllerP1->buttonGruen->wasPressed() == true) ||
        (controllerP2->buttonGruen->wasPressed() == true)) {
        pitendoGE->mainMenu->execute();
    }

} // mainMenu::menuHandler.


// Einmaliger Aufruf, welcher intern die Button-Handler auf menueHandler abgibt.
void optionMenu::menuStart() {
    // Menue darstellen.
    pitendoGE->optionMenu->resetPosition();
    pitendoGE->optionMenu->draw();

    // Klasseneigenen Button-Handler deaktivieren.
    // Wird mittels menuHandler geregelt.
    controllerDeactivate();

    // Bisherige unmittelbar zuvor stattgefundene Button-Aktivitaeten loeschen.
    controllerP1->refresh();
    controllerP2->refresh();

    // Neuen Handler setzen.
    pitendoGE->gameEngineFunktion = &optionMenu::menuHandler;

} // optionMenu::menuStart.


// Dauerhafter Button-Handler.
void optionMenu::menuHandler() {
    // Joystick-Bewegung auslesen.
    bool upC1, downC1, leftC1, rightC1;
    bool upC2, downC2, leftC2, rightC2;
    controllerP1->joystick->getMovement(upC1, downC1, leftC1, rightC1);
    controllerP2->joystick->getMovement(upC2, downC2, leftC2, rightC2);

    // Soll im Menue nach oben oder unten gewandert werden?
    if (upC1 || upC2) {
        pitendoGE->optionMenu->moveUp();
    }
    if (downC1 || downC2) {
        pitendoGE->optionMenu->moveDown();
    }

    // Wurde ein Menue-Punkt angewaehlt?
    if ((controllerP1->buttonGruen->wasPressed() == true) ||
        (controllerP2->buttonGruen->wasPressed() == true)) {
        pitendoGE->optionMenu->execute();
    }
} // optionMenu::menuHandler.

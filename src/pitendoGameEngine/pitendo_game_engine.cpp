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
    this->optionMenu->addEntry("Kalibrierung Joystick Controller 1", &controllerCalibration::calibrateJoystickC1);
    this->optionMenu->addEntry("Kalibrierung Joystick Controller 2", &controllerCalibration::calibrateJoystickC2);
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
    if (loopTimeNew >= this->loopTimeOld) {
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
// #####                        SONDERFUNKTIONEN                            #####
// ##############################################################################

// ---------------------- KLASSE CALIBRATION MANAGER ----------------------------

// Konstruktor.
controllerCalibration::CalibrationManager::CalibrationManager(Controller *controller) {
    this->controller = controller;
    // X-Werte.
    this->adcMinX = 2048;
    this->adcMeanX = 2048;
    this->adcMaxX = 2048;
    this->adcX = 2048;
    // Y-Werte.
    this->adcMinY = 2048;
    this->adcMeanY = 2048;
    this->adcMaxY = 2048;
    this->adcY = 2048;
    // Aktueller Status.
    this->calibrationState = CALIBRATION_START;
    // Verweildauern.
    this->timePause = 60;           // Je laenger, desto mehr Zeit hat Nutzer, um aktuelle Aufgabe zu lesen.
    this->timeMean = 200;           // Je laenger, desto mehr Messpunkte werden genutzt.
    this->timeMinMax = 400;         // Je laenger, desto mehr Messpunkte werden genutzt.
    this->timeNow = 0;
    // Koordination der Bildschirmausgabe.
    this->posY = 1;
} // CalibrationManager::CalibrationManager.


// Destruktor.
controllerCalibration::CalibrationManager::~CalibrationManager() {
    // Aktuell gibt es nichts zu tun.
} // CalibrationManager::CalibrationManager.


// Finale Kalibrierung vornehmen. Plausibilitaetspruefung wird von Klasse Joystick
// durchgefuehrt und Ergebnis rueckgemeldet.
bool controllerCalibration::CalibrationManager::calibrate() {
    return this->controller->joystick->calibration( this->adcMeanX, 
                                                    this->adcMinX, 
                                                    this->adcMaxX,
                                                    this->adcMeanY, 
                                                    this->adcMinY, 
                                                    this->adcMaxY);
} // CalibrationManager::calibrate.



// ------------------------- KALIBRIER-FUNKTIONEN -------------------------------




// Unser Kalibrier-Objekt.
controllerCalibration::CalibrationManager *calibrationManager;

// Kalibrierung des Joysticks. Allgemeine Funktion.
void controllerCalibration::calibrateJoystick() {
    // Allgemeine Variablen.
    int lengthProcessBar = 40;      // Laenge des Fortschrittbalkens in Anzahl Zeichen.
    // Handlungen innerhalb dieser Funktion sind abhaenging von dem Status des 
    // Kalibrierungs-Managers.
    switch (calibrationManager->calibrationState) {
        case CALIBRATION_START:
            // Kalibrierung wurde soeben erst gestartet.
            // Nimm einmalige Aktionen vor und starte folgend mit der Kalibrierung.
            // Bildschirm loeschen.
            clearScreen();
            cout << "Starte Kalibrierung des Controllers ..." << endl << endl;
            calibrationManager->posY += 2;
            // Setze alle Buttons zurueck.
            calibrationManager->controller->refresh();
            // Wechsel in den naechsten Zustand.
            calibrationManager->calibrationState = CALIBRATION_MEAN;
            break;
        case CALIBRATION_MEAN:
            // Kalibrierung des Mittelpunkts.
            if (calibrationManager->timeNow == 0) {
                // Erstmaliger Durchlauf.
                cout << "Kalibrierung des Mittelpunkts. Bitte Joystick nicht bewegen." << endl;
                calibrationManager->posY++;
                // Leeren Fortschrittsbalken zeichnen.
                cout << "[";
                for (int i = 0; i < lengthProcessBar; i++) cout << " ";
                cout << "] 0%";
            }
            else if ((calibrationManager->timeNow > calibrationManager->timePause) &&
                    (calibrationManager->timeNow <= (calibrationManager->timePause + calibrationManager->timeMean))){
                // Messung des Mittelwertes. Davor wird noch eine gewisse Pausenzeit gewartet, damit
                // der Anwender Zeit hat, die vorherige Aufforderung zu lesen und den Joystick entsprechend
                // nach Anweisung zu benutzen.
                // Ermittlung der aktuellen Joystick-Position.
                calibrationManager->controller->joystick->getRawData(calibrationManager->adcX, calibrationManager->adcY);
                // Bildung des Mittelpunktes mittels exponentieller Glaettung.
                float weightingNewValue = 0.3f;
                calibrationManager->adcMeanX = (int)(   (1.0f - weightingNewValue) * (float)(calibrationManager->adcMeanX) + 
                                                        weightingNewValue * (float)(calibrationManager->adcX));
                calibrationManager->adcMeanY = (int)(   (1.0f - weightingNewValue) * (float)(calibrationManager->adcMeanY) + 
                                                        weightingNewValue * (float)(calibrationManager->adcY));
                // Visualisierung am Bildschirm.
                float process = (float)(calibrationManager->timeNow - calibrationManager->timePause) / (float)(calibrationManager->timeMean);
                int posProcessBar = (int)(process * (float)(lengthProcessBar));
                setCursorPosition(2, calibrationManager->posY);
                for (int i = 0; i < posProcessBar; i++) cout << "=";
                if (posProcessBar < lengthProcessBar) cout << ">";
                setCursorPosition(4 + lengthProcessBar, calibrationManager->posY);
                cout << (int)(100.0f * process) << "%";
            }
            else if (calibrationManager->timeNow > (calibrationManager->timePause + calibrationManager->timeMean)) {
                // Kalibrierung des Mittelwerts abgeschlossen.
                // Ausgabe des Ergebnisses.
                calibrationManager->posY += 2;
                setCursorPosition(1, calibrationManager->posY);
                cout << "==> Kalibrierung des Mittelwertes abgeschlossen." << endl; calibrationManager->posY++;
                cout << "   ==> adc-Wert x-Achse: " << calibrationManager->adcMeanX << endl; calibrationManager->posY++;
                cout << "   ==> adc-Wert y-Achse: " << calibrationManager->adcMeanY << endl << endl; calibrationManager->posY += 2;
                // Resette den Timer und wechsele in den naechsten Zustand.
                calibrationManager->timeNow = 0;
                calibrationManager->calibrationState = CALIBRATION_MIN_MAX;
                break;
            }
            calibrationManager->timeNow++;
            break;
        case CALIBRATION_MIN_MAX:
            // Kalibrierung der maximalen Auslenkungen.
            if (calibrationManager->timeNow == 0) {
                // Erstmaliger Durchlauf.
                cout << "Kalibrierung der maximalen Auslenkungen. Bitte Joystick im Kreis bewegen." << endl;
                calibrationManager->posY++;
                // Leeren Fortschrittsbalken zeichnen.
                cout << "[";
                for (int i = 0; i < lengthProcessBar; i++) cout << " ";
                cout << "] 0%";
            }
            else if ((calibrationManager->timeNow > calibrationManager->timePause) &&
                    (calibrationManager->timeNow <= (calibrationManager->timePause + calibrationManager->timeMinMax))){
                // Messung der maximalen Auslenkungen. Davor wird noch eine gewisse Pausenzeit gewartet, damit
                // der Anwender Zeit hat, die vorherige Aufforderung zu lesen und den Joystick entsprechend
                // nach Anweisung zu benutzen.
                // Ermittlung der aktuellen Joystick-Position.
                calibrationManager->controller->joystick->getRawData(calibrationManager->adcX, calibrationManager->adcY);
                // Ueberpruefung, ob neue maximale Auslenkungen gemessen wurden.
                // X-Achse.
                if (calibrationManager->adcX < calibrationManager->adcMeanX) {
                    if (calibrationManager->adcX < calibrationManager->adcMinX) {     // Neukalibrierung notwendig?
                        calibrationManager->adcMinX = calibrationManager->adcX;
                    }
                }
                else {
                    if (calibrationManager->adcX > calibrationManager->adcMaxX) {     // Neukalibrierung notwendig?
                        calibrationManager->adcMaxX = calibrationManager->adcX;
                    }
                }
                // Y-Achse.
                if (calibrationManager->adcY < calibrationManager->adcMeanY) {
                    if (calibrationManager->adcY < calibrationManager->adcMinY) {     // Neukalibrierung notwendig?
                        calibrationManager->adcMinY = calibrationManager->adcY;
                    }
                }
                else {
                    if (calibrationManager->adcY > calibrationManager->adcMaxY) {     // Neukalibrierung notwendig?
                        calibrationManager->adcMaxY = calibrationManager->adcY;
                    }
                }
                // Visualisierung am Bildschirm.
                float process = (float)(calibrationManager->timeNow - calibrationManager->timePause) / (float)(calibrationManager->timeMinMax);
                int posProcessBar = (int)(process * (float)(lengthProcessBar));
                setCursorPosition(2, calibrationManager->posY);
                for (int i = 0; i < posProcessBar; i++) cout << "=";
                if (posProcessBar < lengthProcessBar) cout << ">";
                setCursorPosition(4 + lengthProcessBar, calibrationManager->posY);
                cout << (int)(100.0f * process) << "%";
            }
            else if (calibrationManager->timeNow > (calibrationManager->timePause + calibrationManager->timeMinMax)) {
                // Kalibrierung der maximalen Auslenkungen abgeschlossen.
                // Ausgabe des Ergebnisses.
                calibrationManager->posY += 2;
                setCursorPosition(1, calibrationManager->posY);
                cout << "==> Kalibrierung der maximalen Auslenkungen abgeschlossen." << endl;
                cout << "   ==> maximaler adc-Wert x-Achse: " << calibrationManager->adcMinX << endl;
                cout << "   ==> minimaler adc-Wert x-Achse: " << calibrationManager->adcMaxX << endl << endl;
                cout << "   ==> maximaler adc-Wert y-Achse: " << calibrationManager->adcMinY << endl;
                cout << "   ==> minimaler adc-Wert y-Achse: " << calibrationManager->adcMaxY << endl << endl;
                // Wechsele in den naechsten Zustand.
                calibrationManager->calibrationState = CALIBRATION_READY;
                break;
            }
            calibrationManager->timeNow++;
            break;
        case CALIBRATION_READY:
            // Kalibrierung abgeschlossen.
            // Ermittle einmalig Plausibilitaets-Ergebnis der Kalibrierung und gib
            // Ergebnis am Bildschirm aus.
            cout << "Plausibilisiere Kalibrierungsgroessen ..." << endl;
            if (calibrationManager->calibrate() == true) {
                // Kalibrierung erfolgreich.
                cout << "==> Kalibrierungsgroessen plausibel und erfolgreich abgespeichert." << endl << endl;
            }
            else {
                // Kalibrierung fehlgeschlagen.
                cout << "==> Kalibrierungsgroessen unplausibel! Kalibrierung fehlgeschlagen." << endl << endl;
            }    
            // Informiere Nutzer ueber Moeglichkeit, ins Hauptmenue zurueckzukehren.
            cout << "Roten Button druecken, um ins Menue zurueckzukehren." << endl;
            // Gehe in den Leerlauf.
            calibrationManager->calibrationState = CALIBRATION_IDLE;
            break;
        case CALIBRATION_IDLE:
            // Leerlauf. Mache nichts, bis Nutzer den roten Button drueckt.
            break;
        case CALIBRATION_EXIT:
            // Kalibrierung soll beendet werden.
            // Gib Speicher wieder frei.
            delete calibrationManager;
            // Gehe zurueck in das Optionsmenue.
            optionMenu::menuStart();
            return;
        default:
            // Sollte eigentlich nicht vorkommen.
            cout << "Fehler! Unbekannter Kalibrierungszustand." << endl;
            break;
    }
    // Ueberpruefe, ob Abbruchknopf gedrueckt wurde.
    // Falls ja, dann geh direkt in den Leerlauf-Modus und beende die Kalibrierung.
    if (calibrationManager->controller->buttonRot->wasPressed() == true) {
        calibrationManager->calibrationState = CALIBRATION_EXIT;
    }
} // controllerCalibration::calibrateJoystick.

// Kalibrierung des Joysticks des ersten Controllers. 
// Funktion fuer Funktionspointer des Optionen-Menues.
void controllerCalibration::calibrateJoystickC1() {
    // Objekt fuer Kalibrierung erzeugen.
    calibrationManager = new CalibrationManager(controllerP1);
    // Dauerschleife neu besetzen.
    pitendoGE->gameEngineFunktion = &controllerCalibration::calibrateJoystick;
} // controllerCalibration::calibrateJoystickC1.

// Kalibrierung des Joysticks des zweiten Controllers. 
// Funktion fuer Funktionspointer des Optionen-Menues.
void controllerCalibration::calibrateJoystickC2() {
    // Objekt fuer Kalibrierung erzeugen.
    calibrationManager = new CalibrationManager(controllerP2);
    // Dauerschleife neu besetzen.
    pitendoGE->gameEngineFunktion = &controllerCalibration::calibrateJoystick;
} // controllerCalibration::calibrateJoystickC1.



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


// Diese Funktion wird bei Verlassen des Menues durch die abloesende Funktion aufgerufen.
// Hier ist es zum Beispiel moeglich, die Musik des Haupt-Menues zu beenden.
void mainMenu::menuExit() {
    // Aktuell gibt es nichts zu tun.
    // Wird Musik eingebunden, sieht das jedoch schon anders aus.
} // mainMenu::menuExit.


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


// Diese Funktion wird bei Verlassen des Menues durch die abloesende Funktion aufgerufen.
// Hier ist es zum Beispiel moeglich, die Musik des Option-Menues zu beenden.
void optionMenu::menuExit() {
    // Aktuell gibt es nichts zu tun.
    // Wird Musik eingebunden, sieht das jedoch schon anders aus.
} // optionMenu::menuExit.

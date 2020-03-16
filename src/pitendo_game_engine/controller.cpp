#include "controller.h"

#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

using namespace std;


// Zwei globale Objekte des Typs Controller.
Controller* controllerP1;
Controller* controllerP2;


// ##############################################################################
// #####                        KLASSE BUTTON                               #####
// ##############################################################################


// Konstruktor.
Button::Button(int pinNumber) {
    // WiringPi-Pin-Mode auf INPUT fuer Button-Pin setzen.
    pinMode (pinNumber, INPUT);

    // Interrupt setzen.
    // Memberfunktionen gehen leider nicht, daher muss fuer jeden Button eine eigene
    // Funktion hinterlegt werden.
    switch (pinNumber) {
        case DEF_PIN_BTN_GRUEN_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonGruenC1);
            break;
        case DEF_PIN_BTN_ROT_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonRotC1);
            break;
        case DEF_PIN_BTN_BLAU_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonBlauC1);
            break;
        case DEF_PIN_BTN_GELB_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonGelbC1);
            break;
        case DEF_PIN_BTN_START_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonStartC1);
            break;
        case DEF_PIN_BTN_JOYSTICK_C1:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonJoystickC1);
            break;
        case DEF_PIN_BTN_GRUEN_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonGruenC2);
            break;
        case DEF_PIN_BTN_ROT_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonRotC2);
            break;
        case DEF_PIN_BTN_BLAU_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonBlauC2);
            break;
        case DEF_PIN_BTN_GELB_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonGelbC2);
            break;
        case DEF_PIN_BTN_START_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonStartC2);
            break;
        case DEF_PIN_BTN_JOYSTICK_C2:
            wiringPiISR (pinNumber, INT_EDGE_FALLING, &interruptButtonJoystickC2);
            break;
        default:
            cout << "Undefinierte pinNumber! Button::Button." << endl;
    }

    // Attribute definieren.
    this->pinNumber = pinNumber;
    this->bPressed = false;
    this->buttonFunction = &Button::defaultButtonFunction;
    this->timeLastPressed = 0;

} // Button::Button.


// Destruktor.
Button::~Button() {
    wiringPiISR (pinNumber, INT_EDGE_BOTH, NULL);
} // Button::~Button.


// Setzt den boolschen Wert bPressed zurueck.
void Button::refresh() {
    this->bPressed = false;
} // Button::refresh.


// Ermittelt, ob der Button genau JETZT gedrueckt ist.
// Umsetzung mittels Abfrage des GPIO-Pins.
bool Button::isPressed() {
    int digitalValue = digitalRead(this->pinNumber);
    if (digitalValue == 0) {     // Pull-Up-Widerstand.
        // Button gedrueckt.
        return true;
    }
    else {
        // Button nicht gedrueckt.
        return false;
    }
} // Button::isPressed.


// Ermittelt, ob der Button seit dem letzten Abruf gedrueckt wurde.
// Setzt im Anschluss den boolschen Wert zurueck (refresh).
// Umsetzung mittels Interrupt.
bool Button::wasPressed() {
    bool bReturn;
    if (this->bPressed == true) {
        // Button wurde seit letzter Abfrage gedrueckt.
        // Ueberpruefe, ob Zeit zwischen letzter Abfrage und dieser Abfrage
        // gross genug ist (filtere Tasterprellen heraus).
        unsigned int timePressed = millis();
        // Wurde zufaellig unsigned int Grenze ueberschritten (aller 49 Tage)?
        if (timePressed < this->timeLastPressed) {
            // In diesem seltenen Fall funktioniert Software-Entprellung nicht.
            // Koennte theoretisch noch behoben werden, aber wird nicht benoetigt.
            bReturn = true;
            // Zeit neu abspeichern.
            this->timeLastPressed = timePressed;
        }
        else {
            if ((timePressed - this->timeLastPressed) >= DEF_BUTTON_IDLE) {
                bReturn = true;
                // Zeit neu abspeichern.
                this->timeLastPressed = timePressed;
            }
            else {
                // Es handelt sich um Prellen. 
                // Dies soll nicht als Button-Druck verstanden werden.
                bReturn = false;
            }
        }
        // Button zuruecksetzen.
        this->refresh();
    }
    else {
        // Button wurde seit letzter Abfrage nicht gedrueckt.
        bReturn = false;
    }
    return bReturn;
} // Button::wasPressed.


// Mit dieser Funktion wird jeder neue Button zu Beginn initialisiert.
// Sie macht i.d.R. gar nichts.
void Button::defaultButtonFunction() {
    // Mache einfach nix.
} // Button::defaultButtonFunction.


// Diese Funktion dient dem Hardware-Check.
// Bei Knopfdruck wird etwas auf den Bildschirm geschrieben.
void Button::testButtonFunction() {
    cout << "Button gedrueckt." << endl;
} // Button::testButtonFunction.


// ##############################################################################
// #####                        KLASSE JOYSTICK                             #####
// ##############################################################################


// Konstruktor.
Joystick::Joystick(int channelX, int channelY) {
    // Definition der AD-Wandler-Channels.
    this->channelX = channelX;
    this->channelY = channelY;

    // Festlegung der Joystick-Einstellungen auf Standard-Werte.
    // Hier wird eine Kalibrierung sinnvoll sein. Sinnvoll waere auch eine Abspeicherung
    // der kalibrierten Werte, damit man dies nicht bei jedem Start durchfuehren muss.
    // X-Werte.
    this->adcMinX = 0;
    this->adcMeanX = 2048;
    this->adcMaxX = 4095;       // Es handelt sich um eine 12-Bit-Auflösung am MCP3208. 2^12 = 4096. (-1 wegen Start bei 0)
    // Y-Werte.
    this->adcMinY = 0;
    this->adcMeanY = 2048;
    this->adcMaxY = 4095;

    // Leerlauf-Zeiten fuer Cursor-Bewegungs-Abfrage.
    this->idleUp = 0; 
    this->idleDown = 0;
    this->idleLeft = 0;
    this->idleRight = 0;
} // Joystick::Joystick.


// Destruktor.
Joystick::~Joystick() {

} // Joystick::~Joystick.


// Kalibriert die Joystick-Nulllage sowie die maximalen Auslenkungen.
bool Joystick::calibration(int &adcMeanX, int &adcMinX, int &adcMaxX,
                            int &adcMeanY, int &adcMinY, int &adcMaxY) {
    // Die Kalibrierung an sich muss an anderer Stelle stattfinden, da sie mit Ausgaben am Bildschirm
    // implementiert werden soll und sie diese jedoch nicht nutzen kann.
    // Diese Funktion plausibilisiert lediglich die uebergebenen Werte.
    if ((adcMinX < adcMeanX) && 
        (adcMeanX < adcMaxX) &&
        (adcMinY < adcMeanY) && 
        (adcMeanY < adcMaxY)) {
        // Plausibilitaets-Check i.O.
        this->adcMeanX = adcMeanX;
        this->adcMinX = adcMinX;
        this->adcMaxX = adcMaxX;
        this->adcMeanY = adcMeanY;
        this->adcMinY = adcMinY;
        this->adcMaxY = adcMaxY;
        return true;
    }
    else {
        // Daten unplausibel.
        return false;
    }
} // Joystick::calibration.


void Joystick::getCalibrationParameter(   int &adcMeanX, int &adcMinX, int &adcMaxX,
                                int &adcMeanY, int &adcMinY, int &adcMaxY) {
    adcMeanX = this->adcMeanX;
    adcMinX = this->adcMinX;
    adcMaxX = this->adcMaxX;
    adcMeanY = this->adcMeanY;
    adcMinY = this->adcMinY;
    adcMinY = this->adcMaxY;
} // Joystick::getCalibrationParameter.


// Ermittelt die Rohwerte des 12bit-Analog-Digitalwandlers.
void Joystick::getRawData(int &adcX, int &adcY) {
    // Ermittlung x-Achse.
    unsigned char data[3];
    data[0] = 0b00000110;           // Start-Bit=1, Single=1, D2=0.
    data[1] = this->channelX << 6;  // D1 und D0 je nach zu verwendenden Channel.
    data[2] = 0b00000000;           // Der Rest wird mit Infos ueberschrieben.
    // Schreiben und Lesen ueber SPI.
    wiringPiSPIDataRW(DEF_SPI_CHANNEL, data, 3);
    // Umwandeln.
    adcX = ((data[1]) << 8) + data[2];

    // Ermittlung y-Achse.
    data[0] = 0b00000110;           // Start-Bit=1, Single=1, D2=0.
    data[1] = this->channelY << 6;  // D1 und D0 je nach zu verwendenden Channel.
    data[2] = 0b00000000;           // Der Rest wird mit Infos ueberschrieben.
    // Schreiben und Lesen ueber SPI.
    wiringPiSPIDataRW(DEF_SPI_CHANNEL, data, 3);
    // Umwandeln.
    adcY = ((data[1]) << 8) + data[2];

} // Joystick::getRawData.


// Ermittelt die Joystick-Lage.
void Joystick::getPosition(float &x, float &y) {
    // Versuche haben gezeigt, dass die Sensibilitaet des Joysticks von seiner Position
    // abhaengt. Bewegt man den Joystick nahe des Zentrums, sind kaum Aenderungen erkennbar.
    // Die Werte aendern sich im Randbereich rapide. Eine Linearisierung waere durch eine 
    // entsprechende Versuchsreihe denkbar, wird jedoch noch nicht umgesetzt und die Notwendigkeit
    // abgewartet.

    // Ermittlung der Rohdaten.
    int adcX, adcY;
    this->getRawData(adcX, adcY);

    // X-Achse.
    // Umwandeln des 12-Bit-Wertes in einen float-Wert zwischen -1.0f und 1.0f.
    // Je nach Einbaulage des Joysticks, muss hier noch am Vorzeichen gedreht werden.
    if (adcX < this->adcMeanX) {
        if (adcX < this->adcMinX) {     // Neukalibrierung notwendig?
            this->adcMinX = adcX;
        }
        x = +(  (float)(this->adcMeanX - adcX) / 
                (float)(this->adcMeanX - this->adcMinX));
    }
    else {
        if (adcX > this->adcMaxX) {     // Neukalibrierung notwendig?
            this->adcMaxX = adcX;
        }
        x = -(  (float)(adcX - this->adcMeanX) / 
                (float)(this->adcMaxX - this->adcMeanX));
    }

    // Y-Achse.
    // Umwandeln des 12-Bit-Wertes in einen float-Wert zwischen -1.0f und 1.0f.
    // Je nach Einbaulage des Joysticks, muss hier noch am Vorzeichen gedreht werden.
    if (adcY < this->adcMeanY) {
        if (adcY < this->adcMinY) {     // Neukalibrierung notwendig?
            this->adcMinY = adcY;
        }
        y = -(  (float)(this->adcMeanY - adcY) / 
                (float)(this->adcMeanY - this->adcMinY));
    }
    else {
        if (adcY > this->adcMaxY) {     // Neukalibrierung notwendig?
            this->adcMaxY = adcY;
        }
        y = +(  (float)(adcY - this->adcMeanY) / 
                (float)(this->adcMaxY - this->adcMeanY));
    }
} // Joystick::getPosition.


// Die Funktion "getMovement" bietet die Moeglichkeit, den Cursor fuer Auswahl-Bewegungen
// zu nutzen (z.B. im Hauptmenue.). Durch die regelmaessige Abfrage, wuerde der Cursor 
// mehrere Zeilen springen, obwohl man nur eine Zeile springen moechte. Daher ist eine
// Abfrage-Logik zu implementieren.
// Die Funktion muss bei Benutzung regelmaessig abgefragt werden, da sie auch die Leerlauf-
// zaehler aktualisiert!
void Joystick::getMovement(bool &up, bool &down, bool &left, bool &right) {
    // Variablen-Definition.
    const float joystickThreshold = 0.8f;

    // Abfrage der aktuellen Joystick-Position.
    float x, y;
    this->getPosition(x, y);

    // Jeweilige Abfrage der Positionen.
    // Nach oben.
    if (y > joystickThreshold) {
        if (this->idleUp == 0) {
            // Wert darf angenommen werden.
            up = true;
            this->idleUp = this->nLengthIdle;
        }
        else {
            up = false;
            this->idleUp--;
        }
    }
    else {
        // Leerlaufzaehler zuruecksetzen, da Position verlassen wurde.
        up = false;
        this->idleUp = 0;
        // Kann bei Schwankung um den Threshold-Wert zu ungewollten Ausloesern fuehren.
        // Muss beobachtet werden.
    }

    // Nach unten.
    if (y < ((-1.0f) * joystickThreshold)) {
        if (this->idleDown == 0) {
            // Wert darf angenommen werden.
            down = true;
            this->idleDown = this->nLengthIdle;
        }
        else {
            down = false;
            this->idleDown--;
        }
    }
    else {
        // Leerlaufzaehler zuruecksetzen, da Position verlassen wurde.
        down = false;
        this->idleDown = 0;
        // Kann bei Schwankung um den Threshold-Wert zu ungewollten Ausloesern fuehren.
        // Muss beobachtet werden.
    }

    // Nach links.
    if (x < ((-1.0f) * joystickThreshold)) {
        if (this->idleLeft == 0) {
            // Wert darf angenommen werden.
            left = true;
            this->idleLeft = this->nLengthIdle;
        }
        else {
            left = false;
            this->idleLeft--;
        }
    }
    else {
        // Leerlaufzaehler zuruecksetzen, da Position verlassen wurde.
        left = false;
        this->idleLeft = 0;
        // Kann bei Schwankung um den Threshold-Wert zu ungewollten Ausloesern fuehren.
        // Muss beobachtet werden.
    }

    // Nach rechts.
    if (x > joystickThreshold) {
        if (this->idleRight == 0) {
            // Wert darf angenommen werden.
            right = true;
            this->idleRight = this->nLengthIdle;
        }
        else {
            right = false;
            this->idleRight--;
        }
    }
    else {
        // Leerlaufzaehler zuruecksetzen, da Position verlassen wurde.
        right = false;
        this->idleRight = 0;
        // Kann bei Schwankung um den Threshold-Wert zu ungewollten Ausloesern fuehren.
        // Muss beobachtet werden.
    }

} // Joystick::getMovement.




// ##############################################################################
// #####                        KLASSE CONTROLLER                           #####
// ##############################################################################


// Konstruktur.
Controller::Controller( int pinButtonGruen,
                        int pinButtonRot,
                        int pinButtonBlau,
                        int pinButtonGelb,
                        int pinButtonStart,
                        int pinButtonJoystick,
                        int channelJoystickX,
                        int channelJoystickY,
                        int pinCheck) {
    // Buttons.
    this->buttonGruen = new Button(pinButtonGruen);
    this->buttonRot = new Button(pinButtonRot);
    this->buttonBlau = new Button(pinButtonBlau);
    this->buttonGelb = new Button(pinButtonGelb);
    this->buttonStart = new Button(pinButtonStart);
    this->buttonJoystick = new Button(pinButtonJoystick);

    // Schnellzugriff auf Buttons.
    buttonHandler[0] = buttonGruen;
    buttonHandler[1] = buttonRot;
    buttonHandler[2] = buttonBlau;
    buttonHandler[3] = buttonGelb;
    buttonHandler[4] = buttonStart;
    buttonHandler[5] = buttonJoystick;

    // Joystick.
    this->joystick = new Joystick(channelJoystickX, channelJoystickY);

    // Der Pin "pinCheck" ist auf dem Controller mit Masse kurzgeschlossen.
    // Dadurch wird ein Ein- und Ausstecken des Controllers erkannt.
    // WiringPi-Pin-Mode auf INPUT fuer Check-Pin setzen.
    this->pinCheck = pinCheck;
    pinMode (this->pinCheck, INPUT);

} // Controller::Controller.


// Destruktor.
Controller::~Controller() {

} // Controller::~Controller.


// Setzt die Werte aller Buttons zurueck.
void Controller::refresh() {
    for (int i = 0; i < DEF_NUM_BUTTONS; i++) {
        buttonHandler[i]->refresh();
    }
} // Controller::refresh.


// Fuehrt die Funktionen aller Buttons aus, sollten sie gedrueckt wurden sein.
void Controller::execute() {
    for (int i = 0; i < DEF_NUM_BUTTONS; i++) {
        if (buttonHandler[i]->wasPressed() == true) {
            buttonHandler[i]->buttonFunction();
        }
    }
} // Controller::execute.


// Ueberprueft, ob Controller angeschlossen ist.
bool Controller::isConnected() {
    int digitalValue = digitalRead(this->pinCheck);
    if (digitalValue == 0) {     // Pull-Up-Widerstand.
        // Controller angeschlossen.
        return true;
    }
    else {
        // Controller nicht angeschlossen.
        return false;
    }
} // Controller:isConnected.

// Setzt mit einem Schlag alle Buttonfunktionen.
void Controller::setButtonFunctions(void (*buttonFunctionGruen)(void),
                        void (*buttonFunctionRot)(void),
                        void (*buttonFunctionGelb)(void),
                        void (*buttonFunctionBlau)(void),
                        void (*buttonFunctionStart)(void),
                        void (*buttonFunctionJoystick)(void)) {
    this->buttonGruen->buttonFunction = buttonFunctionGruen;
    this->buttonRot->buttonFunction = buttonFunctionRot;
    this->buttonGelb->buttonFunction = buttonFunctionGelb;
    this->buttonBlau->buttonFunction = buttonFunctionBlau;
    this->buttonStart->buttonFunction = buttonFunctionStart;
    this->buttonJoystick->buttonFunction = buttonFunctionJoystick;
} // Controller::setButtonFunctions.


// ##############################################################################
// #####                        ALLGEMEIN                                   #####
// ##############################################################################


// Funktionen fuer die Interrupts.
// Controller 1.
void interruptButtonGruenC1(void) {
    controllerP1->buttonGruen->bPressed = true;
}
void interruptButtonRotC1(void) {
    controllerP1->buttonRot->bPressed = true;
}
void interruptButtonBlauC1(void) {
    controllerP1->buttonBlau->bPressed = true;
}
void interruptButtonGelbC1(void) {
    controllerP1->buttonGelb->bPressed = true;
}
void interruptButtonStartC1(void) {
    controllerP1->buttonStart->bPressed = true;
}
void interruptButtonJoystickC1(void) {
    controllerP1->buttonJoystick->bPressed = true;
}

// Controller 2.
void interruptButtonGruenC2(void) {
    controllerP2->buttonGruen->bPressed = true;
}
void interruptButtonRotC2(void) {
    controllerP2->buttonRot->bPressed = true;
}
void interruptButtonBlauC2(void) {
    controllerP2->buttonBlau->bPressed = true;
}
void interruptButtonGelbC2(void) {
    controllerP2->buttonGelb->bPressed = true;
}
void interruptButtonStartC2(void) {
    controllerP2->buttonStart->bPressed = true;
}
void interruptButtonJoystickC2(void) {
    controllerP2->buttonJoystick->bPressed = true;
}


// Initialisiert die beiden Controller und deren Funktionalitaeten.
bool controllerSetup() {
    // Initialisierung von wiringPi.
    if (wiringPiSetup() < 0) {
        cout << "WiringPi konnte nicht initialisiert werden!" << endl;
        return false;
    }

    // Initialisierung der SPI-Schnittstelle.
    if (wiringPiSPISetup(DEF_SPI_CHANNEL, DEF_SPI_CLOCK_SPEED) < 0) {
        cout << "WiringPiSPI konnte nicht initialisiert werden!" << endl;
        return false;
    }
    
    // Erzeugung der beiden Controller.
    controllerP1 = new Controller(  DEF_PIN_BTN_GRUEN_C1,
                                    DEF_PIN_BTN_ROT_C1,
                                    DEF_PIN_BTN_BLAU_C1,
                                    DEF_PIN_BTN_GELB_C1,
                                    DEF_PIN_BTN_START_C1,
                                    DEF_PIN_BTN_JOYSTICK_C1,
                                    DEF_CH_JOYSTICK_X_C1,
                                    DEF_CH_JOYSTICK_Y_C1,
                                    DEF_PIN_CHECK_C1);
    controllerP2 = new Controller(  DEF_PIN_BTN_GRUEN_C2,
                                    DEF_PIN_BTN_ROT_C2,
                                    DEF_PIN_BTN_BLAU_C2,
                                    DEF_PIN_BTN_GELB_C2,
                                    DEF_PIN_BTN_START_C2,
                                    DEF_PIN_BTN_JOYSTICK_C2,
                                    DEF_CH_JOYSTICK_X_C2,
                                    DEF_CH_JOYSTICK_Y_C2,
                                    DEF_PIN_CHECK_C2);

    // Ueberpruefe, ob beide Controller angeschlossen sind.
    // Controller 1.
    if (controllerP1->isConnected() == true)
        cout << "Controller #1 ist verbunden." << endl;
    else {
        cout << "Controller #1 ist nicht verbunden!" << endl;
        return false;
    }
    // Controller 2.
    if (controllerP2->isConnected() == true)
        cout << "Controller #2 ist verbunden." << endl;
    else {
        cout << "Controller #2 ist nicht verbunden!" << endl;
        return false;
    }

    return true;
} // controllerSetup.


// Fuehrt die Funktionen beider Controller aus.
// Diese Funktion ist fuer den ButtonHandler gedacht.
void controllerExecute() {
    controllerP1->execute();
    controllerP2->execute();
} // controllerExecute.


// Deaktiviert die klasseneigenen Controller-Callbacks.
// (sinnvoll, wenn Button-Handling durch separate Funktion betrieben wird.)
void controllerDeactivate() {
    controllerP1->setButtonFunctions(   &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction);
    controllerP2->setButtonFunctions(   &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction,
                                        &Button::defaultButtonFunction);
} // controllerDeactivate.


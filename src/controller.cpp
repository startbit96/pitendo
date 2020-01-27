#include "controller.h"

#include <iostream>
#include <wiringPi.h>

using namespace std;

// Zwei globale Objekte des Typs Controller.
Controller* controllerP1;
Controller* controllerP2;


// ##############################################################################
// #####                        KLASSE BUTTON                               #####
// ##############################################################################


// Konstruktor.
Button::Button(int pinNummer) {
    // WiringPi-Pin-Mode auf INPUT fuer Button-Pin setzen.
    pinMode (pinNummer, INPUT);

    // Interrupt setzen.
    switch (pinNummer) {
        case DEF_PIN_BTN_GRUEN_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonGruenC1);
            break;
        case DEF_PIN_BTN_ROT_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonRotC1);
            break;
        case DEF_PIN_BTN_BLAU_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonBlauC1);
            break;
        case DEF_PIN_BTN_GELB_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonGelbC1);
            break;
        case DEF_PIN_BTN_START_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonStartC1);
            break;
        case DEF_PIN_BTN_GRUEN_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonGruenC2);
            break;
        case DEF_PIN_BTN_ROT_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonRotC2);
            break;
        case DEF_PIN_BTN_BLAU_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonBlauC2);
            break;
        case DEF_PIN_BTN_GELB_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonGelbC2);
            break;
        case DEF_PIN_BTN_START_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonStartC2);
            break;
        default:
            cerr << "Undefinierte pinNummer! Button::Button." << endl;
    }

    // Attribute definieren.
    this->pinNummer = pinNummer;
    this->bPressed = false;
    this->buttonFunktion = &Button::defaultButtonFunktion;

} // Button::Button.


// Destruktor.
Button::~Button() {
    wiringPiISR (pinNummer, INT_EDGE_BOTH, NULL);
} // Button::~Button.


// Setzt den boolschen Wert bPressed zurueck.
void Button::refresh() {
    this->bPressed = false;
} // Button::refresh.


// Ermittelt, ob der Button genau JETZT gedrueckt ist.
// Umsetzung mittels Abfrage des GPIO-Pins.
bool Button::isPressed() {
    int digitalWert = digitalRead(this->pinNummer);
    if (digitalWert == 1) {
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
    bool bReturn = this->bPressed;
    this->refresh();
    return bReturn;
} // Button::wasPressed.


// Mit dieser Funktion wird jeder neue Button zu Beginn initialisiert.
void Button::defaultButtonFunktion() {
    // Mache einfach nix.
} // Button::defaultButtonFunktion.


// ##############################################################################
// #####                        KLASSE JOYSTICK                             #####
// ##############################################################################


// Konstruktor.
Joystick::Joystick() {
    //this->joystickButton = &Button(21);
} // Joystick::Joystick.


// Destruktor.
Joystick::~Joystick() {

} // Joystick::~Joystick.


// Kalibriert die Joystick-Nulllage sowie die maximalen Auslenkungen.
bool Joystick::kalibrierung() {
    return true;
} // Joystick::kalibrierung.


// Ermittelt die Joystick-Lage.
void Joystick::getPosition(float &x, float &y) {

} // Joystick::getPosition.

// ##############################################################################
// #####                        KLASSE CONTROLLER                           #####
// ##############################################################################


// Konstruktur.
Controller::Controller( int pinButtonGruen,
                        int pinButtonRot,
                        int pinButtonBlau,
                        int pinButtonGelb,
                        int pinButtonStart) {
    // Buttons.
    this->buttonGruen = new Button(pinButtonGruen);
    this->buttonRot = new Button(pinButtonRot);
    this->buttonBlau = new Button(pinButtonBlau);
    this->buttonGelb = new Button(pinButtonGelb);
    this->buttonStart = new Button(pinButtonStart);

    // Schnellzugriff.
    buttonHandler[0] = buttonGruen;
    buttonHandler[1] = buttonRot;
    buttonHandler[2] = buttonBlau;
    buttonHandler[3] = buttonGelb;
    buttonHandler[4] = buttonStart;

} // Controller::Controller.


// Destruktor.
Controller::~Controller() {

} // Controller::~Controller.


// Setzt die Werte aller Buttons zurueck.
void Controller::refresh() {
    this->buttonGruen->refresh();
    this->buttonRot->refresh();
    this->buttonBlau->refresh();
    this->buttonGelb->refresh();
    this->buttonStart->refresh();
} // Controller::refresh.


// Fuehrt die Funktionen aller Buttons aus, sollten sie gedrueckt wurden sein.
void Controller::execute() {
    for (int i = 0; i < DEF_NUM_BUTTONS; i++) {
        if (buttonHandler[i]->wasPressed() == true) {
            buttonHandler[i]->buttonFunktion();
        }
    }
} // Controller::execute.


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


// Initialisiert die beiden Controller und deren Funktionalitaeten.
bool controllerSetup() {
    // Initialisierung von wiringPi.
    if (wiringPiSetup() < 0) {
        cerr << "WiringPi konnte nicht initialisiert werden!" << endl;
    }
    
    // Erzeugung der beiden Controller.
    controllerP1 = new Controller(  DEF_PIN_BTN_GRUEN_C1,
                                    DEF_PIN_BTN_ROT_C1,
                                    DEF_PIN_BTN_BLAU_C1,
                                    DEF_PIN_BTN_GELB_C1,
                                    DEF_PIN_BTN_START_C1);
    controllerP2 = new Controller(  DEF_PIN_BTN_GRUEN_C2,
                                    DEF_PIN_BTN_ROT_C2,
                                    DEF_PIN_BTN_BLAU_C2,
                                    DEF_PIN_BTN_GELB_C2,
                                    DEF_PIN_BTN_START_C2);

    return true;
}
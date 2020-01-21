#include "controller.h"

#include <iostream>
#include <wiringPi.h>

using namespace std;


// ##############################################################################
// #####                        KLASSE BUTTON                               #####
// ##############################################################################


// Konstruktor.
Button::Button(int pinNummer) {
    // WiringPi-Pin-Mode auf INPUT fuer Button-Pin setzen.
    pinMode (pinNummer, INPUT);

    // Interrupt definieren.
    if (wiringPiISR (pinNummer, INT_EDGE_RISING, this->pressedInterrupt()) < 0) {
        cerr << "Der Button-Interrupt fuer den Pin " << pinNummer << 
                "konnte nicht gesetzt werden!" << endl;
    }

    // Attribute definieren.
    this->pinNummer = pinNummer;
    this->bPressed = false;
    this->buttonFunktion = NULL;
} // Button::Button.


// Destruktor.
Button::~Button() {

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


// Wird durch einen wiringPi-Interrupt aufgerufen.
// Setzt den boolschen Wert bPressed auf true.
void pressedInterrupt(void) {
    this->bPressed = true;
} // Button::pressedInterrupt.


// ##############################################################################
// #####                        KLASSE JOYSTICK                             #####
// ##############################################################################


// Konstruktor.
Joystick::Joystick() {
    this->joystickButton = new Button(21);
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
Controller::Controller() {
    // Initialisierung von wiringPi.
    if (wiringPiSetup() < 0) {
        cerr << "WiringPi konnte nicht initialisiert werden!" << endl;
    };

} // Controller::Controller.


// Destruktor.
Controller::~Controller() {

} // Controller::~Controller.


// Setzt die Werte aller Buttons zurueck.
void Controller::refresh() {
    this->buttonGruen.refresh();
    this->buttonRot.refresh();
    this->buttonGelb.refresh();
    this->buttonBlau.refresh();
    this->buttonStart.refresh();
    this->joystick.joystickButton.refresh();
} // Controller::refresh.
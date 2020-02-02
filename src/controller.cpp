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
        case DEF_PIN_BTN_JOYSTICK_C1:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonJoystickC1);
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
        case DEF_PIN_BTN_JOYSTICK_C2:
            wiringPiISR (pinNummer, INT_EDGE_FALLING, &interruptButtonJoystickC2);
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
    if (digitalWert == 0) {     // Pull-Up-Widerstand.
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
Joystick::Joystick(int channelX, int channelY) {
    // Definition der AD-Wandler-Channels.
    this->channelX = channelX;
    this->channelY = channelY;

    // Festlegung der Joystick-Einstellungen auf Standard-Werte.
    // Hier wird eine Kalibrierung sinnvoll sein. Sinnvoll waere auch eine Abspeicherung
    // der kalibrierten Werte, damit man dies nicht bei jedem Start durchfuehren muss.
    // X-Werte.
    this->adcMinX = 0;
    this->adcMittelX = 2048;
    this->adcMaxX = 4096;       // Es handelt sich um eine 12-Bit-Auflösung am MCP3208. 2^12 = 4096.
    // Y-Werte.
    this->adcMinY = 0;
    this->adcMittelY = 2048;
    this->adcMaxY = 4096;

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
    // Versuche haben gezeigt, dass die Sensibilitaet des Joysticks von seiner Position
    // abhaengt. Bewegt man den Joystick nahe des Zentrums, sind kaum Aenderungen erkennbar.
    // Die Werte aendern sich im Randbereich rapide. Eine Linearisierung waere durch eine 
    // entsprechende Versuchsreihe denkbar, wird jedoch noch nicht umgesetzt und die Notwendigkeit
    // abgewartet.
    // X-Achse.
    unsigned char data[3];
    data[0] = 0b00000110;           // Start-Bit=1, Single=1, D2=0.
    data[1] = this->channelX << 6;  // D1 und D0 je nach zu verwendenden Channel.
    data[2] = 0b00000000;           // Der Rest wird mit Infos ueberschrieben.
    // Schreiben und Lesen ueber SPI.
    wiringPiSPIDataRW(DEF_SPI_CHANNEL, data, 3);
    // Umwandeln.
    int adcX = ((data[1]) << 8) + data[2];
    // Umwandeln des 12-Bit-Wertes in einen float-Wert zwischen -1.0f und 1.0f.
    // Je nach Einbaulage des Joysticks, muss hier noch am Vorzeichen gedreht werden.
    if (adcX < this->adcMittelX) {
        if (adcX < this->adcMinX) {     // Neukalibrierung notwendig?
            this->adcMinX = adcX;
        }
        x = +(  (float)(this->adcMittelX - adcX) / 
                (float)(this->adcMittelX - this->adcMinX));
    }
    else {
        if (adcX > this->adcMaxX) {     // Neukalibrierung notwendig?
            this->adcMaxX = adcX;
        }
        x = -(  (float)(adcX - this->adcMittelX) / 
                (float)(this->adcMaxX - this->adcMittelX));
    }

    // Y-Achse.
    data[0] = 0b00000110;           // Start-Bit=1, Single=1, D2=0.
    data[1] = this->channelY << 6;  // D1 und D0 je nach zu verwendenden Channel.
    data[2] = 0b00000000;           // Der Rest wird mit Infos ueberschrieben.
    // Schreiben und Lesen ueber SPI.
    wiringPiSPIDataRW(DEF_SPI_CHANNEL, data, 3);
    // Umwandeln.
    int adcY = ((data[1]) << 8) + data[2];
    // Umwandeln des 12-Bit-Wertes in einen float-Wert zwischen -1.0f und 1.0f.
    // Je nach Einbaulage des Joysticks, muss hier noch am Vorzeichen gedreht werden.
    if (adcY < this->adcMittelY) {
        if (adcY < this->adcMinY) {     // Neukalibrierung notwendig?
            this->adcMinY = adcY;
        }
        y = -(  (float)(this->adcMittelY - adcY) / 
                (float)(this->adcMittelY - this->adcMinY));
    }
    else {
        if (adcY > this->adcMaxY) {     // Neukalibrierung notwendig?
            this->adcMaxY = adcY;
        }
        y = +(  (float)(adcY - this->adcMittelY) / 
                (float)(this->adcMaxY - this->adcMittelY));
    }
} // Joystick::getPosition.



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


// Ueberprueft, ob Controller angeschlossen ist.
bool Controller::isConnected() {
    int digitalWert = digitalRead(this->pinCheck);
    if (digitalWert == 0) {     // Pull-Up-Widerstand.
        // Controller angeschlossen.
        return true;
    }
    else {
        // Controller nicht angeschlossen.
        return false;
    }
} // Controller:isConnected.


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
        cerr << "WiringPi konnte nicht initialisiert werden!" << endl;
        return false;
    }

    // Initialisierung der SPI-Schnittstelle.
    if (wiringPiSPISetup(DEF_SPI_CHANNEL, DEF_SPI_CLOCK_SPEED) < 0) {
        cerr << "WiringPiSPI konnte nicht initialisiert werden!" << endl;
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
    else
        cout << "Controller #1 ist nicht verbunden!" << endl;
    // Controller 2.
    if (controllerP2->isConnected() == true)
        cout << "Controller #2 ist verbunden." << endl;
    else
        cout << "Controller #2 ist nicht verbunden!" << endl;

    return true;
}
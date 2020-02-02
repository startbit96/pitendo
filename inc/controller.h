/*
################################ CONTROLLER.H #################################
Tim Schwarzbrunn

Die Datei controller.h stellt fuer das Projekt "pitendo" die notwendige 
Verknuepfung des Controllers mit den GPIO-Pins her.

Hierfuer wird die Bibliothek "wiringPi" (http://wiringpi.com/) verwendet.

Die in "wiringPi" definierten Pin-Nummern decken sich nicht mit den Pin-Nummern 
der BCM-Nummerierung der Raspberry-Pi-Website und sind daher vor Anpassen des 
Codes korrekt zu ermitteln.
Hierfuer in das Terminal des RPis den Befehl "gpio readall" eingeben und in 
der Spalte "wPi" die Nummer ablesen.

Nummerierung GPIO-Pins (Stand Januar 2020, Raspberry Pi 3 Model B):

 +-----+-----+---------+------+---+---Pi 3B--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 0 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3B--+---+------+---------+-----+-----+



Aenderungshistorie:
21.01.2020: Aufbau und Implementierung Button-Klasse und -Funktionalitaet.
02.02.2020: Fertige Implementierung der Controller inkl. Buttons und Joysticks.
            Integration der Ueberpruefung, ob Controller verbunden ist.
            Anpassung der Pin-Belegung auf fertig-gelötete Bauteile.
            (Jetzt nichts mehr an der Pin-Belegung aendern.)


###############################################################################
*/

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_


// Anzahl Buttons pro Controller.
#define DEF_NUM_BUTTONS             6

// Pin-Belegung der Buttons.
#define DEF_PIN_BTN_GRUEN_C1        27
#define DEF_PIN_BTN_ROT_C1          24
#define DEF_PIN_BTN_BLAU_C1         26
#define DEF_PIN_BTN_GELB_C1         25
#define DEF_PIN_BTN_START_C1        29
#define DEF_PIN_BTN_JOYSTICK_C1     28
#define DEF_PIN_BTN_GRUEN_C2        4
#define DEF_PIN_BTN_ROT_C2          2
#define DEF_PIN_BTN_BLAU_C2         1
#define DEF_PIN_BTN_GELB_C2         3
#define DEF_PIN_BTN_START_C2        6
#define DEF_PIN_BTN_JOYSTICK_C2     5

// Pin-Belegung der Controller-Erkennung.
#define DEF_PIN_CHECK_C1            23
#define DEF_PIN_CHECK_C2            0

// Definition SPI-Schnittstelle.
#define DEF_SPI_CHANNEL             0
#define DEF_SPI_CLOCK_SPEED         500000

// Channel-Belegung des Analog-Digital-Wandlers.
#define DEF_CH_JOYSTICK_X_C1        1
#define DEF_CH_JOYSTICK_Y_C1        0
#define DEF_CH_JOYSTICK_X_C2        3
#define DEF_CH_JOYSTICK_Y_C2        2



// ##############################################################################
// #####                        KLASSE BUTTON                               #####
// ##############################################################################


class Button {
    public:
        // Konstruktur und Destruktor.
        Button(int pinNummer);
        ~Button();

        // Public-Methoden.
        void refresh();         // Setzt den boolschen Wert bPressed zurueck.
        bool isPressed();       // Ermittelt, ob der Button genau JETZT gedrueckt ist.
                                // Umsetzung mittels Abfrage des GPIO-Pins.
        bool wasPressed();      // Ermittelt, ob der Button seit dem letzten Abruf gedrueckt wurde.
                                // Setzt im Anschluss den boolschen Wert zurueck (refresh).
                                // Umsetzung mittels Interrupt.
        bool bPressed;          // Speichert, ob der Button seit dem letzten Abruf gedrueckt wurde.

        // Public-Attribute.
        void (*buttonFunktion)(void); // Zeiger auf die hinter dem Button liegende Funktion.
                                    // (Bedarf noch unklar, evtl. wird Abfrage des Button-Status und 
                                    // die daraus folgende Aktion direkt im Spiel implementiert.)

        // Statische Methoden.
        static void defaultButtonFunktion();    // Mit dieser Funktion wird jeder neue Button zu beginnt
                                                // initialisiert.
    
    protected:

    private:
        // Private Attribute.
        int pinNummer;          // GPIO-Nummer des digitalen Eingangs.
    
}; // Klasse Button.


// ##############################################################################
// #####                        KLASSE JOYSTICK                             #####
// ##############################################################################


class Joystick {
    public:
        // Konstruktor und Destruktor.
        Joystick(int channelX, int channelY);
        ~Joystick();

        // Public-Methoden.
        bool kalibrierung();    // Kalibriert die Joystick-Nulllage sowie die maximalen Auslenkungen.
        void getPosition(float &x, float &y);   // Ermittelt die Joystick-Lage.
                                                // Rueckgabewert zwischen -1.0f und 1.0f.

    protected:

    private:
        int channelX, channelY;   // Gibt die Channels des AD-Wandlers an.
        int adcMittelX, adcMinX, adcMaxX;   // Fuer die Bestimmung des Ausschlages des Joysticks.
        int adcMittelY, adcMinY, adcMaxY;   // Diese Werte koennen durch die Kalibrierung veraendert werden.
}; // Klasse Joystick.


// ##############################################################################
// #####                        KLASSE CONTROLLER                           #####
// ##############################################################################


class Controller {
    public:
        // Konstruktor und Destruktor.
        Controller(     int pinButtonGruen,
                        int pinButtonRot, 
                        int pinButtonGelb,
                        int pinButtonBlau,
                        int pinButtonStart,
                        int pinButtonJoystick,
                        int channelJoystickX,
                        int channelJoystickY,
                        int pinCheck);
        ~Controller();

        // Public-Methoden.
        void refresh();     // Setzt die Werte aller Buttons zurueck.
        void execute();     // Fuehrt die Funktionen aller Buttons aus, sollten
                            // sie gedrueckt wurden sein.
        bool isConnected(); // Ueberprueft, ob Controller angeschlossen ist.

        // Public-Attribute.
        // Buttons.
        Button* buttonGruen;
        Button* buttonRot;
        Button* buttonGelb;
        Button* buttonBlau;
        Button* buttonStart;
        Button* buttonJoystick;

        // Joystick.
        Joystick* joystick;
        
    protected:

    private:
        // Fuer den schnelleren Zugriff.
        Button* buttonHandler[DEF_NUM_BUTTONS];

        // Pin-Nummer der Anschluss-Ueberpruefung.
        int pinCheck;

}; // Klasse Controller.


// ##############################################################################
// #####                        ALLGEMEIN                                   #####
// ##############################################################################


// Zwei globale Objekte des Typs Controller.
extern Controller* controllerP1;
extern Controller* controllerP2;

// Funktionen fuer die Interrupts.
// Controller 1.
void interruptButtonGruenC1(void);
void interruptButtonRotC1(void);
void interruptButtonBlauC1(void);
void interruptButtonGelbC1(void);
void interruptButtonStartC1(void);
void interruptButtonJoystickC1(void);
// Controller 2.
void interruptButtonGruenC2(void);
void interruptButtonRotC2(void);
void interruptButtonBlauC2(void);
void interruptButtonGelbC2(void);
void interruptButtonStartC2(void);
void interruptButtonJoystickC2(void);

// Initialisiert die beiden Controller und deren Funktionalitaeten.
bool controllerSetup();

#endif /*!_CONTROLLER_H_*/
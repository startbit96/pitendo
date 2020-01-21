/*
################################ CONTROLLER.H #################################
Tim Schwarzbrunn

Die Klasse controller.h stellt fuer das Projekt "pintendo" die notwendige 
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


###############################################################################
*/

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

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

        // Public-Attribute.
        void* buttonFunktion(void); // Zeiger auf die hinter dem Button liegende Funktion.
                                    // (Bedarf noch unklar, evtl. wird Abfrage des Button-Status und 
                                    // die daraus folgende Aktion direkt im Spiel implementiert.)
    protected:

    private:
        // Private-Methoden.
        void pressedInterrupt(void);    // Wird durch einen wiringPi-Interrupt aufgerufen.
                                        // Setzt den boolschen Wert bPressed auf true.

        // Private Attribute.
        int pinNummer;          // GPIO-Nummer des digitalen Eingangs.
        bool bPressed;          // Speichert, ob der Button seit dem letzten Abruf gedrueckt wurde.
    
}; // Klasse Button.

class Joystick {
    public:
        // Konstruktor und Destruktor.
        Joystick();
        ~Joystick();

        // Public-Methoden.
        bool kalibrierung();    // Kalibriert die Joystick-Nulllage sowie die maximalen Auslenkungen.
        void getPosition(float &x, float &y);   // Ermittelt die Joystick-Lage.

        // Public-Attribute.
        // ################################################## PINS DEFINIEREN!!! 
        Button joystickButton;   // Button, welcher bei Hineindruecken des Joysticks aktiviert wird.

    protected:

    private:
    
}; // Klasse Joystick.

class Controller {
    public:
        // Konstruktor und Destruktor.
        Controller();
        ~Controller();

        // Public-Methoden.
        void refresh();     // Setzt die Werte aller Buttons zurueck.

        // Public-Attribute.
        // Buttons.
        Button buttonGruen(0);
        Button buttonRot(1);
        Button buttonGelb(2);
        Button buttonBlau(3);
        Button buttonStart(4);

        // Joystick.
        Joystick joystick();

    protected:

    private:

}; // Klasse Controller.

#endif /*!_CONTROLLER_H_*/
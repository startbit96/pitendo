# pitendo
Gaming on Raspberry Pi in C++.

# To Do:
1. Bereitstellung der Controller inkl. der Ansteuerung. --> CHECK
2. Menü. --> CHECK
3. Zeichen-Funktionen. --> CHECK
4. Beispiel-Spiel. --> CHECK
5. Joystick-Kalibrierung. --> CHECK
6. Musik für Startbildschirm und Menüs. --> OFFEN.


# Installations-Anweisung.
1. Empfohlene Ordnerstruktur: Erstelle im Ordner "Documents" den Ordner "repos" und wechsele in diesen.
```
cd Documents
mkdir repos
cd repos
```

2. Downloade das Git-Repository und wechsele in den gedownloadeten Ordner.
```
git clone https://github.com/startbit96/pitendo.git
cd pitendo
```

3. Installiere den Terminal-Emulator "xterm":
```
sudo apt-get install xterm
```

4. Aktiviere die quadratische Schriftart "square":
```
mkdir ~/.fonts
cp res/square.ttf ~/.fonts/
fc-cache
```

5. Das Projekt wird mittels cmake gelinkt und kompiliert. Gegebenfalls muss cmake heruntergeladen werden und die IDE angepasst werden.

6. Aktivieren von SPI auf dem Raspberry Pi (unter Interface).
```
sudo raspi-config
```



# Übersicht der Dateien.
1. framework: display.h und display.cpp

Diese Dateien stellen grundlegende Zeichenfunktionen sowie eine Klasse für das Erstellen von Menüs analog Haupt- und Pausenmenü zur Verfügung.


2. framework: controller.h und controller.cpp

Diese Dateien binden die Controller ein. Es werden Funktionen zur Taster-Drücker-Erkennung sowie zur Positionsauslesung des Joysticks gestellt.


3. framework: pitendo_game_engine.h und pitendo_game_engine.cpp

Diese Dateien beinhalten die Game-Engine, welche den zeitlichen Ablauf im Programm steuert (FPS-Einstellung, ...).
Außerdem werden hier das Hauptmenü und das Optionsmenü (Pausenmenü) gemanaged.


4. Hauptdatei: pitendo.cpp

Diese Datei ist die grundlegende Pitendo-Datei, welche den Pitendo-Start, die Pitendo-Dauerschleife und das Pitendo-Herunterfahren beinhaltet.
Außerdem werden an dieser Stelle die Spiele eingebunden.


5. Vorlage-Spiel: template.h und template.cpp

Diese beiden Dateien zeigen beispielhaft die Implementierung eines Spiels in Pitendo auf. 
Es werden die Grundfunktionen definiert, welche jedes Spiel zu stellen hat.
Es kann als Vorlage fuer die Entwicklung eines eigenen Spiels genutzt werden. 
(Bitte hierfür kopieren und umbenennen. Nicht in der ursprünglichen Datei arbeiten.)

Positionen, welche anzupassen sind, sind mit "##### MUSS ANGEPASST WERDEN #####" gekennzeichnet.



# Grundlegende Funktionen (Details siehe Header-Files).
1. Ermittlung der Terminalgröße als Spielfeldbegrenzer.
```
pitendeGE->screenWidth
pitendoGE->screenHeight
```

2. Nutzung der Buttons eines Controllers.
Möglichkeit 1: Hinterlegen der Funktionen, welche bei Buttondrücken aufgerufen werden sollen.
Memberfunktion der Objekte controllerP1, controllerP2:
```
void setButtonFunctions(    void (*buttonFunktionGruen)(void),
                            void (*buttonFunktionRot)(void),
                            void (*buttonFunktionGelb)(void),
                            void (*buttonFunktionBlau)(void),
                            void (*buttonFunktionStart)(void),
                            void (*buttonFunktionJoystick)(void));
```
Ausführen, sofern gedrückt:
```
void execute();
```

Möglichkeit 2: Manuelle Abfrage der Button-Zustände.
Zugriff über Memberfunktion jedes Buttons.
z.B. 
```
controllerP1->buttonGruen->wasPressed()
```

Vorteil Möglichkeit 1:
- Nutzung der Implementierung

Nachteil Möglichkeit 1:
- Es können keine Memberfunktionen von Objekten verknüpft werden.
- Es müssen Funktionen ohne Rückgabe- und Übergabewert sein (void ()()).

Einsatzmöglichkeit:
- Im Spiel.


Vorteil Möglichkeit 2:
- Individuelle Nutzung.

Nachteil Möglichkeit 2:
- Vermutlich ein wenig mehr Code-Aufwand, da bereits bestehende Implementierung nicht genutzt wird.

Einsatzmöglichkeit:
- In Menüs.


3. Nutzung des Joysticks.
Zugriff auf Joystick:
```
controllerP1->joystick
controllerP2->joystick
```

Digitale Nutzung über Memberfunktion (wurde Ausschlag über Schwellwert festgestellt?):
```
void getMovement(bool &up, bool &down, bool &left, bool &right);
```

Analoge Nutzung über Memberfunktion:
```
void getPosition(float &x, float &y);
```

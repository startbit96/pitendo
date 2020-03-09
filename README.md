# pitendo
Gaming on Raspberry Pi in C++.

# To Do:
1. Bereitstellung der Controller inkl. der Ansteuerung.
2. Menü.
3. Zeichen-Funktionen.
4. Beispiel-Spiel??


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
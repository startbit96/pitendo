#ifndef _DISPLAY_H_
#define _DISPLAY_H_


/*
############################### DISPLAY.H #####################################
Tim Schwarzbrunn

Diese Datei dient der Bereitstellung von Funktionalitaeten im Terminal.
Grundlage fuer viele Operationen sind Escape-Sequenzen.
(siehe z.B. http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html)

###############################################################################
*/

#include <string.h>


bool setupDisplay();
void moveCursorUp(int numberOfSteps = 1);
void moveCursorDown(int numberOfSteps = 1);
void moveCursorLeft(int numberOfSteps = 1);
void moveCursorRight(int numberOfSteps = 1);
void setCursorPosition(int x, int y);
void getCursorPosition(int &x, int &y);
void getCursorLimits(int &x, int &y);
void clearScreen();

class Display {
    public:
        int width, height;
    protected:

    private:

};

class Menue {
    public:
        void addMenueEntry();
        void deleteMenueEntry();
        void execute();
        void moveUp;
        void moveDown;

    protected:

    private:
        int numberOfEntries;
        int currentPosition;
};

class MenueEintrag {
    public:
        string menueText;
        void (*menueFunktion)(void); // Zeiger auf die Funktion hinter dem Menueeintrag.
    protected:

    private:

}




#endif /*!_DISPLAY_H_*/
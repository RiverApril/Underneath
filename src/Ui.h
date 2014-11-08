//
//  Ui.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Ui__
#define __Underneath__Ui__

#include "Geometry.h"

extern std::vector<std::string> consoleBuffer;

void debug(std::string s);
void print(std::string s);

namespace Ui {

    typedef int color;


    const color C_DARK_BLACK = 0xE8;
    const color C_DARK_RED = 0x1;
    const color C_DARK_GREEN = 0x2;
    const color C_DARK_YELLOW = 0x3;
    const color C_DARK_BLUE = 0x4;
    const color C_DARK_MAGENTA = 0x5;
    const color C_DARK_CYAN = 0x6;
    const color C_DARK_WHITE = 0x7;

    const color C_LIGHT_BLACK = 0x8;
    const color C_LIGHT_RED = 0x9;
    const color C_LIGHT_GREEN = 0xA;
    const color C_LIGHT_YELLOW = 0xB;
    const color C_LIGHT_BLUE = 0xC;
    const color C_LIGHT_MAGENTA = 0xD;
    const color C_LIGHT_CYAN = 0xE;
    const color C_LIGHT_WHITE = 0xF;

    const color C_LIGHT_GRAY = C_DARK_WHITE;
    const color C_DARK_GRAY = C_LIGHT_BLACK;

    const color C_WHITE = C_LIGHT_WHITE;
    const color C_BLACK = C_DARK_BLACK;


    extern const color COLOR_DEFAULT_ENTITY;

    void initNCurses();
    void initColorPairs();

    void exitProgram();

    void setTerminalSizeVar();

    void setColor(color c, int attr = 0);

    extern unsigned long tick;

    /*extern double ms;

    extern int fps;
    extern long l;
    extern long lastL;
    extern long startTime;
    extern int frames;*/

    extern bool limitedColorMode;

    extern Point2 terminalSize;


    class Menu {
    public:

        Menu(bool temp);

        virtual ~Menu() {}

        void _openUi(Menu* oldMenu);
        void _closeUi(Menu* newMenu);
        void _handleInput(int i);
        void _update();

        void printConsole(int topY = terminalSize.y-4, int bottomY = terminalSize.y-1);

        virtual void openUi(Menu* oldMenu) {}
        virtual void closeUi(Menu* newMenu) {}
        virtual void handleInput(int i) {}
        virtual void update() {}
        
        bool isTemp = false;

    protected:

        Menu* parentMenu = nullptr;

    };
    
    
    
    
    extern Menu* currentMenu;
    
    void changeMenu(Menu* newMenu);
    
    
}

#endif /* defined(__Underneath__Ui__) */

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

extern vector<string> consoleBuffer;

void debug(string s);
void debugf(string format, ...);
void print(string s);

namespace Ui {

    typedef char Color;

    extern Color C_DARK_BLACK;
    extern Color C_DARK_RED;
    extern Color C_DARK_GREEN;
    extern Color C_DARK_YELLOW;
    extern Color C_DARK_BLUE;
    extern Color C_DARK_MAGENTA;
    extern Color C_DARK_CYAN;
    extern Color C_DARK_WHITE;

    extern Color C_LIGHT_BLACK;
    extern Color C_LIGHT_RED;
    extern Color C_LIGHT_GREEN;
    extern Color C_LIGHT_YELLOW;
    extern Color C_LIGHT_BLUE;
    extern Color C_LIGHT_MAGENTA;
    extern Color C_LIGHT_CYAN;
    extern Color C_LIGHT_WHITE;

    extern Color C_LIGHT_GRAY;
    extern Color C_DARK_GRAY;

    extern Color C_WHITE;
    extern Color C_BLACK;




    extern const Color COLOR_DEFAULT_ENTITY;

    void initNCurses();
    void initColorPairs();

    void exitProgram();

    void setTerminalSizeVar();

    void setColor(Color fg, Color bg = C_BLACK, int attr = 0);

    extern unsigned long tick;

    /*extern double ms;

    extern int fps;
    extern long l;
    extern long lastL;
    extern long startTime;
    extern int frames;*/

    extern bool limitedColorMode;

    extern Point2 terminalSize;

    extern bool consoleInputMode;
    extern string consoleInput;
    extern int consoleScroll;

    extern bool printDebugEnabled;
    
    
}

#endif /* defined(__Underneath__Ui__) */

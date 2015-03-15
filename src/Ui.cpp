//
//  Ui.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "MenuGame.h"
#include "Entity.h"
#include "AiEntity.h"
#include "Player.h"
#include "Math.h"

vector<string> consoleBuffer;

void debug(string s){
    if(Ui::printDebugEnabled){
    	console("DEBUG: "+s);
    }
}

void console(string s){
    size_t lastI = 0;
    for(size_t i=0;i<s.length();i++){
        if(s[i] == '\n'){
            consoleBuffer.push_back(s.substr(lastI+1, i-lastI));
            lastI = i;
        }else if(i-lastI > Ui::terminalSize.x - 4){
            consoleBuffer.push_back(s.substr(lastI, i-lastI));
            lastI = i;
        }
    }
    consoleBuffer.push_back(s.substr(lastI));
}


void debugf(string format, ...){
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    debug(buff);
    va_end(args);
}


void consolef(string format, ...){
	char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    console(buff);
    va_end(args);
}

string formatString(string format, ...){
    string s = "";
    
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    s+=buff;
    va_end(args);

    return s;
}

namespace Ui {

    Color C_DARK_BLACK = 0x0; // white when background is black
    Color C_DARK_RED = 0x1;
    Color C_DARK_GREEN = 0x2;
    Color C_DARK_YELLOW = 0x3;
    Color C_DARK_BLUE = 0x4;
    Color C_DARK_MAGENTA = 0x5;
    Color C_DARK_CYAN = 0x6;
    Color C_DARK_WHITE = 0x7;

    Color C_LIGHT_BLACK = 0x8;
    Color C_LIGHT_RED = 0x9;
    Color C_LIGHT_GREEN = 0xA;
    Color C_LIGHT_YELLOW = 0xB;
    Color C_LIGHT_BLUE = 0xC;
    Color C_LIGHT_MAGENTA = 0xD;
    Color C_LIGHT_CYAN = 0xE;
    Color C_LIGHT_WHITE = 0xF;

    Color C_LIGHT_GRAY = C_DARK_WHITE;
    Color C_DARK_GRAY = C_LIGHT_BLACK;

    Color C_WHITE = C_LIGHT_WHITE;
    Color C_BLACK = C_DARK_BLACK;

    unsigned long tick = 0;

    /*double ms = 0;

    int fps = 0;
    long l = 0;
    long lastL = 0;
    long startTime = 0;
    int frames = 0;*/

    bool limitedColorMode = true;

    Point2 terminalSize;

    bool printDebugEnabled = true;

    bool consoleInputMode = false;
    string consoleInput = "";
    int consoleScroll = 0;

    void initNCurses() {

        setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        setTerminalSizeVar();
        keypad(stdscr, TRUE);
        noecho();
        cbreak();
        curs_set(0);
        timeout(-1);
        ESCDELAY = 1;


        limitedColorMode = COLORS<256;

		if (limitedColorMode){
			swap(C_DARK_RED, C_DARK_BLUE);
			swap(C_LIGHT_RED, C_LIGHT_BLUE);
			swap(C_DARK_CYAN, C_DARK_YELLOW);
			swap(C_LIGHT_CYAN, C_LIGHT_YELLOW);
		}

        initColorPairs();

        refresh();

    }

    void initColorPairs() {

        short a = 0;

        for(short i = 0; i<0x10; i++) {
            for(short j = 0; j<0x10; j++) {
            	init_pair(a, j, i);
                a++;
            }
        }

    }

    void setTerminalSizeVar(){
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(Color fg, Color bg, int attr) {
        if(limitedColorMode){
            if(fg == 0x8){
                fg = 0x7;
            }else if(fg >= 0x8){
                fg -= 0x8;
            }
            if(bg == 0x8){
                bg = 0x7;
            }else if(bg >= 0x8){
                bg -= 0x8;
            }
        }
        attrset(COLOR_PAIR(fg + (bg*0x10)) | attr);
    }
}

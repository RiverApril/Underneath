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
    	print("DEBUG: "+s);
    }
}

void print(string s){
    if(s.find('\n') == string::npos){

        size_t lastI = 0;
        for(size_t i=0;i<s.length();i++){
            if(s[i] == '\n'){
                consoleBuffer.push_back(s.substr(lastI, i-lastI));
                lastI = i+1;
            }
        }
        consoleBuffer.push_back(s.substr(lastI));

    }else{
    	consoleBuffer.push_back(s);
    }
}

namespace Ui {

    const color COLOR_DEFAULT_ENTITY = C_LIGHT_RED;

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

        initColorPairs();

        refresh();

    }

    void initColorPairs() {

        int a = 0;

        for(int i = 0; i<0x10; i++) {
            for(int j = 0; j<0x10; j++) {
            	init_pair(a, j, i);
                a++;
            }
        }

    }

    void setTerminalSizeVar(){
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(color fg, color bg, int attr) {
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

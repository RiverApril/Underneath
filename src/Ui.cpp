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

std::vector<std::string> consoleBuffer;

void debug(std::string s){
    print("DEBUG: "+s);
}

void print(std::string s){
    consoleBuffer.push_back(s);
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

    Menu* currentMenu;

    void changeMenu(Menu* newMenu) {
        if(currentMenu != nullptr){
        	currentMenu->_closeUi(newMenu);
        }
        newMenu->_openUi(currentMenu);
        currentMenu = newMenu;
    }

    void initNCurses() {

        setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        setTerminalSizeVar();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
        //timeout(-1);
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

    void exitProgram() {
        running = false;
        delete currentMenu;
        endwin();
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


    Menu::Menu(bool temp) {
        isTemp = temp;
    }

    void Menu::_openUi(Menu* oldMenu) {
        //timeout(inputTimeout);
        move(0, 0);
        clrtobot();
        refresh();
        openUi(oldMenu);
        update();
        refresh();
        if(isTemp){
            parentMenu = oldMenu;
        }
    }

    void Menu::_closeUi(Menu* newMenu){
        closeUi(newMenu);
        if(isTemp && newMenu == parentMenu){
            delete this;
        }
    }

    void Menu::_handleInput(int in) {
        switch (in) {
            case KEY_RESIZE:
                setTerminalSizeVar();
                move(0, 0);
                clrtobot();
                break;

            case '`':
                debug("Debug Test, Rand: "+std::to_string(rand()));
                break;
        }
        handleInput(in);
    }

    void Menu::printConsole(int topY, int bottomY){
        setColor(C_WHITE);
        int j = 1;
        for(int i=bottomY;i>=topY;i--){
            move(i, 0);
            clrtoeol();
            if(((int)(consoleBuffer.size())-j) < 0){
                break;
            }
            printw(consoleBuffer[consoleBuffer.size()-j].c_str());
            j++;
        }
    }

    void Menu::_update() {

        update();

        tick++;

        /*l = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        ms = (l - lastL)/1000.0;
        lastL = l;

        frames++;
        if(l - startTime > 1000000) {
            startTime = l;
            fps = frames;
            frames = 0;
        }*/

        

    }

    void Menu::printCenter(int y, std::string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2));
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }

    void Menu::printCenterOffset(int y, int xOff, std::string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2)+xOff);
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }
}

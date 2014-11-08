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


        limitedColorMode = COLORS<256;

        initColorPairs();

        refresh();

    }

    void initColorPairs() {

        for(int i = 0; i<COLORS; i++) {
            init_pair(i, i, 0);
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

    void setColor(color c, int attr) {
        if(limitedColorMode) {
            switch(c){
                case C_DARK_BLACK:
                case C_LIGHT_BLACK:
                    c = COLOR_BLACK;
                    break;
                case C_DARK_BLUE:
                case C_LIGHT_BLUE:
                    c = COLOR_BLUE;
                    break;
                case C_DARK_CYAN:
                case C_LIGHT_CYAN:
                    c = COLOR_CYAN;
                    break;
                case C_DARK_GREEN:
                case C_LIGHT_GREEN:
                    c = COLOR_GREEN;
                    break;
                case C_DARK_MAGENTA:
                case C_LIGHT_MAGENTA:
                    c = COLOR_MAGENTA;
                    break;
                case C_DARK_RED:
                case C_LIGHT_RED:
                    c = COLOR_RED;
                    break;
                case C_DARK_WHITE:
                case C_LIGHT_WHITE:
                    c = COLOR_WHITE;
                    break;
                case C_DARK_YELLOW:
                case C_LIGHT_YELLOW:
                    c = COLOR_YELLOW;
                    break;
                default:
                    c  = COLOR_BLACK;
                    break;

            }
        }
        attrset(COLOR_PAIR(c) | attr);
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
        if(isTemp && newMenu == parentMenu){
            delete this;
        }
        closeUi(newMenu);
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
}

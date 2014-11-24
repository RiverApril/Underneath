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
void print(string s);

namespace Ui {

    typedef char color;


    /*const color C_DARK_BLACK = 0xE8;
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
    const color C_BLACK = C_DARK_BLACK;*/

    const color C_DARK_BLACK = 0x0; // white when background is black
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

    void setColor(color fg, color bg = C_BLACK, int attr = 0);

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

        void printCenter(int y, string s, ...);
        void printCenterOffset(int y, int xOff, string s, ...);
        
        bool isTemp = false;

        bool menuOnlyExecute(string commandName, vector<string>arguments, string argumentsRaw){
            return false;
        }

        string menuOnlyCommands(){
            return "";
        }

        string menuOnlyCommandHelp(string name){
			return "";
        }

        Menu* parentMenu = nullptr;

    protected:

    };

    bool execute(string commandRaw, Menu* currentMenu);

    struct Command {
        Command(){}

        virtual bool acceptableName(string name){
            return name.compare(defaultName()) == 0;
        }
        virtual bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            return false;
        }

        virtual string help(){
			return "Undefined Command";
        }

        virtual string usage(){
            return "Undefined Command";
        }
        virtual string defaultName(){
            return "Undefined";
        }
    };

    extern vector<Command*> commandList;
    
    void initCommandList();
    
    
    extern Menu* currentMenu;

    extern bool printDebugEnabled;
    
    void changeMenu(Menu* newMenu);
    
    
}

#endif /* defined(__Underneath__Ui__) */

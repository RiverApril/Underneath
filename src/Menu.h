//
//  Menu.h
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Menu__
#define __Underneath__Menu__

#include "Ui.h"

typedef char yesNo;

extern yesNo aYes;
extern yesNo aNo;
extern yesNo aUndefined;

const char C_CODE_DARK_BLACK = '0';
const char C_CODE_DARK_RED = '1';
const char C_CODE_DARK_GREEN = '2';
const char C_CODE_DARK_YELLOW = '3';
const char C_CODE_DARK_BLUE = '4';
const char C_CODE_DARK_MAGENTA = '5';
const char C_CODE_DARK_CYAN = '6';
const char C_CODE_DARK_WHITE = '7';

const char C_CODE_LIGHT_BLACK = '8';
const char C_CODE_LIGHT_RED = '9';
const char C_CODE_LIGHT_GREEN = 'A';
const char C_CODE_LIGHT_YELLOW = 'B';
const char C_CODE_LIGHT_BLUE = 'C';
const char C_CODE_LIGHT_MAGENTA = 'D';
const char C_CODE_LIGHT_CYAN = 'E';
const char C_CODE_LIGHT_WHITE = 'F';

const char C_CODE_LIGHT_GRAY = C_CODE_DARK_WHITE;
const char C_CODE_DARK_GRAY = C_CODE_LIGHT_BLACK;

const char C_CODE_WHITE = C_CODE_LIGHT_WHITE;
const char C_CODE_BLACK = C_CODE_DARK_BLACK;

namespace Ui{

    string colorCode(char c);

    class Menu;

    class Menu {
    private:

        static const int defaultConsoleHeight = 8;

    public:

        Menu() {}

        virtual ~Menu() {
            
        }

        void _openUi();
        void _closeUi();
        void _handleInput(int i);
        void _update();
        bool consoleHandleInput(int i);

        void printConsole(int topY = Ui::terminalSize.y-defaultConsoleHeight, int bottomY = Ui::terminalSize.y-1);

        virtual bool openUi() {return true;}
        virtual void closeUi() {}
        virtual void handleInput(int i) {}
        virtual void update() {}

        static void printCenter(int y, string s, ...);
        static void printCenterOffset(int y, int xOff, string s, ...);

        bool menuOnlyExecute(string commandName, vector<string>arguments, string argumentsRaw){
            return false;
        }

        string menuOnlyCommands(){
            return "";
        }

        string menuOnlyCommandHelp(string name){
            return "";
        }
        
        Menu* subMenu = nullptr;
        Menu* surMenu = nullptr;

        void openMenu(Menu* newMenu){

            this->_closeUi();

            this->subMenu = newMenu;
            newMenu->surMenu = this;

            newMenu->_openUi();

        }

        void closeThisMenu(){
            this->_closeUi();

            this->surMenu->subMenu = nullptr;

            this->surMenu->_openUi();

            delete this;

        }

        void closeAllMenus(){
            markAllForClosure = true;
            closeThisMenu();
        }
        
    protected:

        bool printConsoleByDefault = true;
        bool markAllForClosure = false;
        
    };
    
}

#endif /* defined(__Underneath__Menu__) */

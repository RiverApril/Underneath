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

namespace Ui{
    
    class Menu;

    class Menu {
    private:

        static const int defaultConsoleHeight = 8;

    public:

        Menu() {}

        ~Menu() {
            
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

//
//  Menu.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Menu.h"
#include "Command.h"

yesNo aYes = 'y';
yesNo aNo = 'n';
yesNo aUndefined = '?';

namespace Ui {
    

    void Menu::_openUi() {
        if(openUi()){
        	update();
        	refresh();
        }
    }

    void Menu::_closeUi(){
        closeUi();
    }

    void Menu::_handleInput(int in) {

        if(subMenu != nullptr){
            subMenu->_handleInput(in);
        }else{
            switch (in) {
                case KEY_RESIZE:
                    setTerminalSizeVar();
                    move(0, 0);
                    clrtobot();
                    break;
            }
            if(!consoleHandleInput(in)){
                switch (in) {
                    case '`':
                        consoleInputMode = true;
                        break;

                    default:
                        handleInput(in);
                        break;
                }
            }
        }
    }

    bool Menu::consoleHandleInput(int in){
        if(consoleInputMode){
            switch (in) {
                case '`':
                    consoleInputMode = false;
                    break;

                case KEY_BACKSPACE:
                case 8: //Backspace
                case 127: //Delete
                    if(consoleInput.length() > 0){
                        consoleInput = consoleInput.substr(0, consoleInput.length()-1);
                    }
                    break;

                case KEY_ENTER:
                case 13:
                case '\n':
                    consoleInputMode = false;
                    execute(consoleInput, this);
                    consoleInput = "";
                    consoleScroll = 0;
                    break;

                case KEY_UP:
                    consoleScroll--;
                    break;

                case KEY_DOWN:
                    consoleScroll++;
                    break;

                default:
                    if((in>=32 && in<=126)){
                        consoleInput += in;
                    }else{
                    	return false;
                    }
            }
            return true;
        }
        return false;
    }

    void Menu::printConsole(int topY, int bottomY){
        setColor(C_WHITE);
        int j = 1;
        for(int i=bottomY-1;i>=topY;i--){
            move(i, 2);
            clrtoeol();
            move(i, 2);
            if(((int)(consoleBuffer.size())-j) < 0){
                break;
            }
            auto p = consoleBuffer.size()-j+consoleScroll;
            if(p < consoleBuffer.size()){
                printw(consoleBuffer[p].c_str());
            }
            j++;
        }
        mvprintw(topY, 0, (consoleScroll > -((int)consoleBuffer.size())+(bottomY-topY))?"^":" ");
        mvprintw(bottomY-1, 0, (consoleScroll < 0)?"v":" ");

        move(bottomY, 2);
        clrtoeol();
        if(consoleInputMode){
            Ui::setColor(C_DARK_GREEN);
            mvprintw(bottomY, 2, "> %s", consoleInput.c_str());
            Ui::setColor(C_LIGHT_GREEN, C_BLACK, A_BLINK);
            mvprintw(bottomY, 4+(int)consoleInput.length(), "_");
        }
    }

    void Menu::_update() {
        
        if(subMenu != nullptr){
            subMenu->_update();
        }else{

            if(printConsoleByDefault){
                if(!consoleInputMode){
                    move(Ui::terminalSize.y-defaultConsoleHeight, 0);
                    clrtobot();
                }
            }

            update();
            tick++;

            if(printConsoleByDefault){
                if(consoleInputMode){
                    printConsole();
                }else{
                    move(Ui::terminalSize.y-defaultConsoleHeight, 0);
                    clrtobot();
                }
            }

        }

        
    }
    
    void Menu::printCenter(int y, string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2));
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }
    
    void Menu::printCenterOffset(int y, int xOff, string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2)+xOff);
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }
    
}

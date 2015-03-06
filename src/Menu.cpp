//
//  Menu.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Menu.h"
#include "Command.h"
#include "MenuMain.h"

yesNo aYes = 'y';
yesNo aNo = 'n';
yesNo aUndefined = '?';

namespace Ui {

    string colorCode(const char c){
        string s = "&";
        s += c;
        return s;
    }
    

    void Menu::_openUi() {
        if(markAllForClosure && !(dynamic_cast<MenuMain*>(this))){
            closeThisMenu();
            return;
        }
        if(openUi()){
        	update();
        	refresh();
        }
    }

    void Menu::_closeUi(){
        if(markAllForClosure){
            if(surMenu != nullptr){
                surMenu->markAllForClosure = true;
            }
        }
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
                    Commands::execute(consoleInput, this);
                case KEY_ESCAPE:
                    consoleInputMode = false;
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
                        consoleInput += (char)in;
                    }
                    break;
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
            if((((int)consoleBuffer.size())-j) < 0){
                break;
            }
            int p = ((int)consoleBuffer.size())-j+(consoleScroll);
            if(p < consoleBuffer.size()){
                bool lookingForCode = false;
                for(char c : consoleBuffer[p]){
                    if(lookingForCode){
                        lookingForCode = false;
                        if(c == '&'){
                            addch('&');
                        }else{
                            Color color = 0;
                            switch (c) {
                                case C_CODE_DARK_BLACK:
                                    color = C_DARK_BLACK;
                                    break;
                                case C_CODE_DARK_RED:
                                    color = C_DARK_RED;
                                    break;
                                case C_CODE_DARK_GREEN:
                                    color = C_DARK_GREEN;
                                    break;
                                case C_CODE_DARK_YELLOW:
                                    color = C_DARK_YELLOW;
                                    break;
                                case C_CODE_DARK_BLUE:
                                    color = C_DARK_BLUE;
                                    break;
                                case C_CODE_DARK_MAGENTA:
                                    color = C_DARK_MAGENTA;
                                    break;
                                case C_CODE_DARK_CYAN:
                                    color = C_DARK_CYAN;
                                    break;
                                case C_CODE_DARK_WHITE:
                                    color = C_DARK_WHITE;
                                    break;
                                case C_CODE_LIGHT_BLACK:
                                    color = C_LIGHT_BLACK;
                                    break;
                                case C_CODE_LIGHT_RED:
                                    color = C_LIGHT_RED;
                                    break;
                                case C_CODE_LIGHT_GREEN:
                                    color = C_LIGHT_GREEN;
                                    break;
                                case C_CODE_LIGHT_YELLOW:
                                    color = C_LIGHT_YELLOW;
                                    break;
                                case C_CODE_LIGHT_BLUE:
                                    color = C_LIGHT_BLUE;
                                    break;
                                case C_CODE_LIGHT_MAGENTA:
                                    color = C_LIGHT_MAGENTA;
                                    break;
                                case C_CODE_LIGHT_CYAN:
                                    color = C_LIGHT_CYAN;
                                    break;
                                case C_CODE_LIGHT_WHITE:
                                    color = C_LIGHT_WHITE;
                                    break;
                                    
                                default:
                                    color = C_WHITE;
                                    break;
                            }
                            setColor(color);
                        }
                    }else if(c == '&'){
                        lookingForCode = true;
                    }else{
                    	addch(c);
                    }
                }
                setColor(C_WHITE);
            }
            j++;
        }
        mvprintw(topY, 0, (consoleScroll > -(consoleBuffer.size())+(bottomY-topY))?"^":" ");
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
                    //move(Ui::terminalSize.y-defaultConsoleHeight, 0);
                    //clrtobot();
                }
            }

        }

        
    }
    
    void Menu::printCenter(int y, string format, ...){
        string s = "";

        char buff[256];
        va_list args;
        va_start(args, format);
        vsprintf(buff, format.c_str(), args);
        s+=buff;
        va_end(args);

        mvprintw(y, (terminalSize.x/2)-(((int)s.length()-1)/2), s.c_str());
    }
    
    void Menu::printCenterOffset(int y, int xOff, string format, ...){
        string s = "";

        char buff[256];
        va_list args;
        va_start(args, format);
        vsprintf(buff, format.c_str(), args);
        s+=buff;
        va_end(args);

        mvprintw(y, (terminalSize.x/2)-(((int)s.length()-1)/2)+xOff, s.c_str());
    }
    
}

//
//  Menu.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Menu.hpp"
#include "Command.hpp"
#include "MenuMain.hpp"

//yesNo aYes = 'y';
//yesNo aNo = 'n';
//yesNo aUndefined = '?';

namespace Ui {

    /*string colorCode(const char c) {
        string s = "&";
        s += c;
        s += C_BLACK;
        return s;
    }*/

    string colorCode(const unsigned char fgc, const unsigned char bgc) {
        string s = "&";
        if(limitedColorMode){
            s += (char)(unsigned char)(fgc | (bgc << 2));
        }else{
            s += (char)(unsigned char)(fgc | (bgc << 4));
        }
        //debugf("code: 0x%x, 0x%x, 0x%x", s[1], bgc, fgc);
        return s;
    }

    void Menu::_openUi() {
        if (markAllForClosure && !(dynamic_cast<MenuMain*> (this))) {
            closeThisMenu();
            return;
        }
        if (openUi()) {
            update();
            refresh();
        }
    }

    void Menu::_closeUi() {
        if (markAllForClosure) {
            if (surMenu != nullptr) {
                surMenu->markAllForClosure = true;
            }
        }
        closeUi();
    }

    void Menu::_handleInput(int in) {

        if (subMenu != nullptr) {
            subMenu->_handleInput(in);
        } else {
            switch (in) {
                case KEY_RESIZE:
                    setTerminalSizeVar();
                    move(0, 0);
                    clrtobot();
                    break;
            }
            if (!consoleHandleInput(in)) {
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

    bool Menu::consoleHandleInput(int in) {
        if (consoleInputMode) {
            switch (in) {
                case '`':
                    consoleInputMode = false;
                    break;

                case KEY_BACKSPACE:
                case 8: //Backspace
                case 127: //Delete
                    if (consoleInput.length() > 0) {
                        consoleInput = consoleInput.substr(0, consoleInput.length() - 1);
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
                    if ((in >= 32 && in <= 126)) {
                        consoleInput += (char) in;
                    }
                    break;
            }
            return true;
        }
        return false;
    }

    void Menu::printConsole(int topY, int bottomY) {
        setColor(C_WHITE);
        int j = 1;
        for (int i = bottomY - 1; i >= topY; i--) {
            move(i, 2);
            clrtoeol();
            move(i, 2);
            if ((((int) consoleBuffer.size()) - j) < 0) {
                break;
            }
            int p = ((int) consoleBuffer.size()) - j + (consoleScroll);
            if (p < consoleBuffer.size()) {
                /*bool lookingForCode = false;
                for (char c : consoleBuffer[p]) {
                    addChColor(c, &lookingForCode);
                }
                setColor(C_WHITE);*/
                int len = stringLengthWithColored(consoleBuffer[p]) / (terminalSize.x - 2);
                i -= len;
                i -= printMultiLineColoredString(i, 2, consoleBuffer[p]) - 1;
                i += len;
            }
            j++;
        }
        mvprintw(topY, 0, (consoleScroll > -(consoleBuffer.size())+(bottomY - topY)) ? "^" : " ");
        mvprintw(bottomY - 1, 0, (consoleScroll < 0) ? "v" : " ");

        move(bottomY, 2);
        clrtoeol();
        if (consoleInputMode) {
            Ui::setColor(C_DARK_GREEN);
            mvprintw(bottomY, 2, "> %s", consoleInput.c_str());
            Ui::setColor(C_LIGHT_GREEN, C_BLACK, A_BLINK);
            mvprintw(bottomY, 4 + (int) consoleInput.length(), "_");
        }
    }

    void Menu::_update() {

        if (subMenu != nullptr) {
            subMenu->_update();
        } else {

            if (printConsoleByDefault) {
                if (!consoleInputMode) {
                    move(Ui::terminalSize.y - defaultConsoleHeight, 0);
                    clrtobot();
                }
            }

            update();
            tick++;

            if (printConsoleByDefault) {
                if (consoleInputMode) {
                    printConsole();
                } else {
                    //move(Ui::terminalSize.y-defaultConsoleHeight, 0);
                    //clrtobot();
                }
            }

        }


    }

    void Menu::printCenter(int y, string format, ...) {
        string s = "";

        char buff[256];
        va_list args;
        va_start(args, format);
        vsprintf(buff, format.c_str(), args);
        s += buff;
        va_end(args);

        mvprintw(y, (terminalSize.x / 2)-(((int) s.length() - 1) / 2), s.c_str());
    }

    void Menu::printCenterOffset(int y, int xOff, string format, ...) {
        string s = "";

        char buff[256];
        va_list args;
        va_start(args, format);
        vsprintf(buff, format.c_str(), args);
        s += buff;
        va_end(args);

        mvprintw(y, (terminalSize.x / 2)-(((int) s.length() - 1) / 2) + xOff, s.c_str());
    }

}

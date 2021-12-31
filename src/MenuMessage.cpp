//
//  MenuMessage.cpp
//  Underneath
//
//  Created by Emily Atlee on 11/25/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#include "MenuMessage.hpp"

namespace Ui {

    MenuMessage::MenuMessage(string message, Menu* nextMenu) : Menu() {
        this->message = {message};
        this->nextMenu = nextMenu;
    }

    MenuMessage::MenuMessage(vector<string> message, Menu* nextMenu) : Menu() {
        this->message = message;
        this->nextMenu = nextMenu;
    }

    void MenuMessage::handleInput(int in) {
        switch (in) {
            case '\n':
            case ' ':
            case KEY_ESCAPE:
                if(nextMenu){
                    closeOnReopen = true;
                    openMenu(nextMenu);
                    return;
                }
                closeThisMenu();
                return;

            default:
                break;
        }
    }

    void MenuMessage::update() {
        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        int y = (int)(terminalSize.y - (message.size()+3)) / 2;
        for(string line : message){
        	printCenter(y++, line);
        }
        y+=2;
        printCenter(y, "Press enter to continue...");
    }
}

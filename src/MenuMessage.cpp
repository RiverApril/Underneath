//
//  MenuMessage.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuMessage.hpp"

namespace Ui {

    MenuMessage::MenuMessage(string message) : Menu() {
        this->message = {message};
    }

    MenuMessage::MenuMessage(vector<string> message) : Menu() {
        this->message = message;
    }

    void MenuMessage::handleInput(int in) {
        switch (in) {
            case '\n':
            case ' ':
            case KEY_ESCAPE:
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
        int y = (int)(terminalSize.y - message.size()) / 2;
        for(string line : message){
        	printCenter(y++, line);
        }
        y+=2;
        printCenter(y, "Press enter to continue...");
    }
}

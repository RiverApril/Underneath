//
//  MenuMessage.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuMessage.h"

namespace Ui {

    MenuMessage::MenuMessage(string message) : Menu(){
        this->message = message;
    }

    void MenuMessage::handleInput(int in){
        switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
                closeThisMenu();
                return;

            default:
                break;
        }
    }

    void MenuMessage::update(){
        setColor(C_WHITE);
        printCenter(terminalSize.y/2-2, message);
        printCenter(terminalSize.y/2, "Press enter to continue...");
    }
}

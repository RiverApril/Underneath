//
//  MenuTempMessage.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuTempMessage.h"

namespace Ui {

    MenuTempMessage::MenuTempMessage(string message) : Menu(){
        this->message = message;
    }

    void MenuTempMessage::handleInput(int in){
        switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
                closeThisMenu();
                break;

            default:
                break;
        }
    }

    void MenuTempMessage::update(){
        setColor(C_WHITE);
        printCenter(terminalSize.y/2-2, message);
        printCenter(terminalSize.y/2, "Press enter to continue...");
    }
}

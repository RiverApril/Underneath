//
//  MenuYesNo.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuYesNo.h"

namespace Ui {

    MenuYesNo::MenuYesNo(string question, yesNo* answer, bool canEscape) : Menu() {
        this->question = question;
        this->answer = answer;
        this->canEscape = canEscape;
        *answer = aUndefined;
    }

    void MenuYesNo::handleInput(int in) {
        switch (in) {
            case 'Y':
            case 'y':
                *answer = aYes;
                closeThisMenu();
                break;

            case 'N':
            case 'n':
                *answer = aNo;
                closeThisMenu();
                break;

            case KEY_ESCAPE:
                if (canEscape) {
                    *answer = aUndefined;
                    closeThisMenu();
                }
                break;

            default:
                break;
        }
    }

    void MenuYesNo::update() {
        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        printCenter(terminalSize.y / 2 - 2, question);
        printCenter(terminalSize.y / 2, "  Y / N ?");
    }
}

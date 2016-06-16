//
//  MenuYesNo.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuYesNo.hpp"

namespace Ui {

    MenuYesNo::MenuYesNo(string question, yesNo* answer, bool canEscape) : Menu() {
        this->question = question;
        this->answer = answer;
        this->canEscape = canEscape;
        *answer = aUndefined;
    }
    
    MenuYesNo::MenuYesNo(string question, function<void(yesNo)> exe, bool canEscape) : Menu() {
        this->question = question;
        this->answer = nullptr;
        this->exe = exe;
        this->canEscape = canEscape;
    }

    void MenuYesNo::handleInput(int in) {
        switch (in) {
            case 'Y':
            case 'y':
                if(answer){
                    *answer = aYes;
                }else{
                    exe(aYes);
                }
                closeThisMenu();
                break;

            case 'N':
            case 'n':
                if(answer){
                    *answer = aNo;
                }else{
                    exe(aNo);
                }
                closeThisMenu();
                break;

            case KEY_ESCAPE:
                if (canEscape) {
                    if(answer){
                        *answer = aUndefined;
                    }else{
                        exe(aUndefined);
                    }
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

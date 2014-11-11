//
//  MenuTempYesNo.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuTempYesNo.h"

namespace Ui {

    MenuTempYesNo::MenuTempYesNo(string question, bool* answer) : Menu(true){
        this->question = question;
        this->answer = answer;
    }

    void MenuTempYesNo::handleInput(int in){
        switch (in) {
            case 'Y':
            case 'y':
                *answer = true;
                changeMenu(parentMenu);
                break;

            case 'N':
            case 'n':
                *answer = false;
                changeMenu(parentMenu);
                break;
                
            default:
                break;
        }
    }

    void MenuTempYesNo::update(){
        printCenter(terminalSize.y/2-2, question);
        printCenter(terminalSize.y/2, "Y / N ?");
    }
}
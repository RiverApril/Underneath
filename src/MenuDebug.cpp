//
//  MenuDebug.cpp
//  Underneath
//
//  Created by Emily Atlee on 2/18/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "MenuDebug.hpp"
#include "ItemGenerator.hpp"
#include "Controls.hpp"
#include "MenuMessage.hpp"

namespace Ui {

    MenuDebug::MenuDebug() : Menu() {

    }


    void MenuDebug::handleInput(int in){
        if(in == KEY_ESCAPE){
            closeThisMenu();
            return;
        }
    }

    void MenuDebug::update() {
        closeThisMenu();
    }

}

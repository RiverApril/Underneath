//
//  MenuCredits.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/30/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#include "MenuCredits.hpp"

namespace Ui {

    MenuCredits::MenuCredits() : Menu() {

    }

    void MenuCredits::handleInput(int in) {
        if (in == KEY_ESCAPE) {
            closeThisMenu();
            return;
        }
    }

    void MenuCredits::update() {
        printCenter(1, "C R E D I T S");

        printCenter(3, "- Lead Developer / Programmer / Artist -");
        printCenter(4, "Braeden Atlee");
    }
    
}

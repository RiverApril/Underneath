//
//  main.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved->
//

#include "Ui.h"
#include "UiMenuMain.h"
#include "Tile.h"

int main(int argc, const char * argv[]) {

    initTiles();

    Ui::initNCurses();

    Ui::changeMenu(new Ui::MenuMain());

    running = true;
    while (running) {
        Ui::currentMenu->_handleInput(getch());
        Ui::currentMenu->_update();
    }

    Ui::exitProgram();

    return 0;
}


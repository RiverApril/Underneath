//
//  main.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Command.h"
#include "MenuMain.h"
#include "Tile.h"
#include "Math.h"

int main(int argc, const char** argv) {


    //cout << endl << "Press enter to start Underneath..." << endl;
    //cin.ignore();

    if (!GetCurrentDir(workingDirectory, sizeof(workingDirectory))){
        return errno;
    }



    debug("Working Directory: "+(string(workingDirectory)));
    UnderneathDir = "Underneath";
    WorldsDir = UnderneathDir+"/"+"Worlds";

    initTiles();

    Ui::initNCurses();

    Ui::initCommandList();

    Ui::MenuMain* mainMenu = new Ui::MenuMain();
    mainMenu->_openUi();

    running = true;
    while (running) {
        mainMenu->_handleInput(getch());
        mainMenu->_update();
    }

    endwin();

    delete mainMenu;

    return 0;
}


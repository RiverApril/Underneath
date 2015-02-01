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
#include "Ranged.h"

int main(int argc, const char** argv) {


    //////

    Item* original = new Ranged(1, "Ranged Weapon", 1, 1, 10);
    cout << "Made Ranged* stored into Item*" << endl;
    Item* cloned = Item::clone(original);
    cout << "Cloned Ranged* stored in Item* to new Item* using Item::clone()" << endl;
    Ranged* rOriginal = dynamic_cast<Ranged*>(original);
    Ranged* rCloned = dynamic_cast<Ranged*>(cloned);
    cout << "casted original range = " << rOriginal->range << endl;
    cout << "casted cloned range = " << rCloned->range << endl;
    
    //////


    cout << endl << "Press enter to start Underneath..." << endl;
    cin.ignore();

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


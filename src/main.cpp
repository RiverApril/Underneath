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
#include "Utility.h"
#include "ItemGenerator.h"

int main(int argc, const char** argv) {


    if (!GetCurrentDir(workingDirectory, sizeof(workingDirectory))){
        return errno;
    }


    cout << endl << "Press enter to start Underneath..." << endl;
    cin.ignore();



    debug("Working Directory: "+(string(workingDirectory)));

    CustomWorkingDirectory = FileUtility::readTextFile("customUnderneathWorkingDirectory.txt", "");
	CustomWorkingDirectory = CustomWorkingDirectory.substr(0, CustomWorkingDirectory.find_last_of('/')+1);
    UnderneathDir = CustomWorkingDirectory+"Underneath";
    WorldsDir = UnderneathDir+"/"+"worlds";
    ArtDir = UnderneathDir+"/"+"art";

    if(CustomWorkingDirectory.length() > 0){
    	debug("Custom Working Directory: "+CustomWorkingDirectory);
    }

    Tiles::initTiles();
    ItemGenerator::initWeaponsTemplates();
    Arts::loadArts();
    Ui::initNCurses();
    Commands::initCommandList();



    Ui::MenuMain* mainMenu = new Ui::MenuMain();
    mainMenu->_openUi();

    running = true;
    while (running) {
        mainMenu->_handleInput(getch());
        mainMenu->_update();
    }

    endwin();

    delete mainMenu;

    Arts::cleanup();
    Tiles::cleanup();
    Commands::cleanup();
    ItemGenerator::cleanup();

    return 0;
}


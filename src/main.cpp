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
#include "EnemyGenerator.h"
#include "Level.h"

int main(int argc, const char** argv) {


    if (!GetCurrentDir(workingDirectory, sizeof(workingDirectory))){
        return errno;
    }


    cout << endl << "Press enter to start Underneath..." << endl;
    cin.ignore();




    debug("Working Directory: "+(string(workingDirectory)));

    CustomWorkingDirectory = FileUtility::readTextFile("customUnderneathWorkingDirectory.txt", "./");
	CustomWorkingDirectory = CustomWorkingDirectory.substr(0, CustomWorkingDirectory.find_last_of('/')+1);
    UnderneathDir = CustomWorkingDirectory;
    WorldsDir = UnderneathDir+"worlds";
    ArtDir = UnderneathDir+"art";

    if(CustomWorkingDirectory.length() > 0){
    	debug("Custom Working Directory: "+CustomWorkingDirectory);
    }

    Tiles::initTiles();
    ItemGenerator::initItemTemplates();
    Arts::loadArts();
    Ui::initNCurses();
    Commands::initCommands();
    EnemyGenerator::initEnemies();


    Ui::MenuMain* mainMenu = new Ui::MenuMain();
    mainMenu->_openUi();

    running = true;
    while (running) {
        mainMenu->_handleInput(getch());
        mainMenu->_update();
    }

    endwin();

    delete mainMenu;

    EnemyGenerator::cleanupEnemies();
    Arts::cleanupArts();
    Tiles::cleanupTiles();
    Commands::cleanupCommands();
    ItemGenerator::cleanupItemTemplates();


    return 0;
}


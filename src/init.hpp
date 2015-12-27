//
//  init.hpp
//  Underneath
//
//  Created by Braeden Atlee on 11/10/15.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Command.hpp"
#include "MenuMain.hpp"
#include "Tile.hpp"
#include "Math.hpp"
#include "ItemRanged.hpp"
#include "Utility.hpp"
#include "ItemGenerator.hpp"
#include "EnemyGenerator.hpp"
#include "Level.hpp"
#include "Settings.hpp"
#include "Random.hpp"

Ui::MenuMain* mainMenu;


bool* init(int argc, char* argv[]);
void update();
int cleanup();


bool* init(int argc, char* argv[]){
    if (!GetCurrentDir(workingDirectory, sizeof (workingDirectory))) {
        exit(errno);
    }


    string s1 = ("Working Directory: " + (string(workingDirectory)));
    string s2 = "";

    CustomWorkingDirectory = Utility::readTextFile("customUnderneathWorkingDirectory.txt", "./");
    CustomWorkingDirectory = CustomWorkingDirectory.substr(0, CustomWorkingDirectory.find_last_of('/') + 1);
    UnderneathDir = CustomWorkingDirectory;
    WorldsDir = UnderneathDir + "worlds";
    ArtDir = UnderneathDir + "art";

    if (CustomWorkingDirectory.length() > 0) {
        s2 = ("Custom Working Directory: " + CustomWorkingDirectory);
    }

    Settings::loadSettings(UnderneathDir + "settings.txt");

    debug("Underneath Start");

    Tiles::initTiles();
    Arts::loadArts();
    ItemGenerator::initItemTemplates();
    Ui::initNCurses(false);
    Commands::initCommands();
    EnemyGenerator::initEnemies();
    Random::setup();

    debug(s1);
    debug(s2);


    mainMenu = new Ui::MenuMain();
    mainMenu->_openUi();
    
    running = true;

    return &running;
}

void update(){
    mainMenu->_handleInput(getchSafe());
    mainMenu->_update();
}

int cleanup(){
    endwin();

    delete mainMenu;

    Settings::saveSettings(UnderneathDir + "settings.txt");
    EnemyGenerator::cleanupEnemies();
    Arts::cleanupArts();
    Tiles::cleanupTiles();
    Commands::cleanupCommands();
    ItemGenerator::cleanupItemTemplates();

    return 0;
}

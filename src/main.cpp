//
//  main.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
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

int main(int argc, const char** argv) {


    if (!GetCurrentDir(workingDirectory, sizeof (workingDirectory))) {
        return errno;
    }


    debug("Working Directory: " + (string(workingDirectory)));

    CustomWorkingDirectory = Utility::readTextFile("customUnderneathWorkingDirectory.txt", "./");
    CustomWorkingDirectory = CustomWorkingDirectory.substr(0, CustomWorkingDirectory.find_last_of('/') + 1);
    UnderneathDir = CustomWorkingDirectory;
    WorldsDir = UnderneathDir + "worlds";
    ArtDir = UnderneathDir + "art";

    if (CustomWorkingDirectory.length() > 0) {
        debug("Custom Working Directory: " + CustomWorkingDirectory);
    }

    Tiles::initTiles();
    Arts::loadArts();
    ItemGenerator::initItemTemplates();
    Ui::initNCurses();
    Commands::initCommands();
    EnemyGenerator::initEnemies();
    Settings::loadSettings(UnderneathDir + "settings.txt");


    Ui::MenuMain* mainMenu = new Ui::MenuMain();
    mainMenu->_openUi();

    running = true;
    while (running) {
        mainMenu->_handleInput(getch());
        mainMenu->_update();
    }

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

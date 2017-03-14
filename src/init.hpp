//
//  init.hpp
//  Underneath
//
//  Created by Braeden Atlee on 11/10/15.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"
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
#include "Audio.hpp"
#include "Offers.hpp"


Ui::MenuMain* mainMenu;


bool* init(int argc, char* argv[]);
void update();
int cleanup();


bool* init(int argc, char* argv[]){
    
#ifdef __APPLE__
    string home = string(getenv("HOME"));
    string appdata = home + "/Library/Application Support/Underneath/";
#elif _WIN32
    string appdata = string(getenv("APPDATA"));
    appdata += "/Underneath/";
#else
    string appdata = "./";
#endif
    
    char runningDirectory[FILENAME_MAX];
    
    if (!GetCurrentDir(runningDirectory, sizeof (runningDirectory))) {
        exit(errno);
    }

    
    string s1 = ("Running Directory: " + (string(runningDirectory)));
    string s2 = "";
    
    string customDataDirectory = Utility::readTextFile("customUnderneathDataDirectory.txt", "");
    if(customDataDirectory.length() > 0){
        customDataDirectory = customDataDirectory.substr(0, customDataDirectory.find_last_of('/') + 1);
        UnderneathDir = customDataDirectory;
        s2 = "Custom Data Directory Read: " + UnderneathDir;
    }else{
        UnderneathDir = appdata;
        s2 = "Default Data Directory: " + UnderneathDir;
    }
    
    WorldsDir = UnderneathDir + "worlds";
    ArtDir = UnderneathDir + "art";
    AudioDir = UnderneathDir + "audio";
    
    mkdir(UnderneathDir.c_str(), 0777);

    Settings::loadSettings(UnderneathDir + "settings.txt");

    debug("Underneath Start");

    Tiles::initTiles();
    Arts::loadArts();
    ItemGenerator::initItemTemplates();
    Ui::initNCurses(false);
    Commands::initCommands();
    EnemyGenerator::initEnemies();
    Offers::initOffers();
    Random::setup();
    Audio::initAudio();

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
    Audio::update();
}

int cleanup(){
    Settings::saveSettings(UnderneathDir + "settings.txt");

    endwin();

    delete mainMenu;
    
    EnemyGenerator::cleanupEnemies();
    Arts::cleanupArts();
    Tiles::cleanupTiles();
    Commands::cleanupCommands();
    ItemGenerator::cleanupItemTemplates();
    Audio::cleanupAudio();

    return 0;
}

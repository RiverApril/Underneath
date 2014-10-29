//
//  World.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "World.h"


World::World(){

}

World::~World(){
    delete currentPlayer;
    delete currentLevel;
}

namespace WorldLoader {

    World* loadedWorld;

    bool exists(std::string name){
        return false;
    }
    
    bool load(std::string name){
        return false;
    }

    bool create(std::string name){

        loadedWorld = new World();

        loadedWorld->currentLevel = new Level();
        loadedWorld->currentLevel->generate(static_cast<unsigned int>(time(NULL)));

        loadedWorld->currentPlayer = new Player("PlayerNameHere", '@', Point2Zero, Ui::C_WHITE);
        loadedWorld->currentLevel->newEntity(loadedWorld->currentPlayer);

        return true;
    }
}
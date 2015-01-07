//
//  World.h
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__World__
#define __Underneath__World__

class Player;
class Level;

#include "Global.h"
#include "Geometry.h"
#include "Player.h"

class World {

public:

    World(std::string n);

    ~World();

    vector<string> levels;

    Level* currentLevel;
    Player* currentPlayer;
    string name = "";
    double worldTime = 0.0;

};

namespace WorldLoader {
    bool exists(std::string name);
    World* load(std::string name, string optionalStartLevel = "");
    bool save(World* loadedWorld);
    World* create(std::string name, Abilities<int> playerAbilities);
    bool changeLevel(World* world, Point2 entrance, string newName);
    bool deleteWorld(std::string name);
    unsigned char* readData(FILE* file);

}

#include "Player.h"
#include "Level.h"

#endif /* defined(__Underneath__World__) */

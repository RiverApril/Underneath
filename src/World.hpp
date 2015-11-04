//
//  World.h
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__World__
#define __Underneath__World__

class EntityPlayer;
class Level;

#include "Global.hpp"
#include "Geometry.hpp"
#include "EntityPlayer.hpp"

class World {
public:

    World(string n);

    ~World();

    vector<string> levels;

    Level* currentLevel;
    EntityPlayer* currentPlayer;
    string name = "";
    double worldTime = 0.0;
    double worldLastTime = 0.0;
    unsigned int seed = 0;

};

namespace WorldLoader {
    bool exists(string name);
    World* load(World* world, string name, string optionalStartLevel = "");
    bool save(World* loadedWorld);
    World* create(string name, Abilities<int> playerAbilities);
    bool changeLevel(World* world, Point2 entrance, string newName);
    bool deleteWorld(string name);
    unsigned char* readData(FILE* file);

}

#include "EntityPlayer.hpp"
#include "Level.hpp"

#endif /* defined(__Underneath__World__) */

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

class World {

public:

    World(std::string n);

    ~World();

    shared_ptr<std::vector<shared_ptr<Level>>> levels;

    shared_ptr<Level> currentLevel;
    shared_ptr<Player> currentPlayer;
    shared_ptr<std::string> name;
    unsigned long worldTime = 0;

};

namespace WorldLoader {
    bool exists(std::string name);
    shared_ptr<World> load(std::string name);
    bool save(shared_ptr<World> loadedWorld);
    shared_ptr<World> create(std::string name);
    bool deleteWorld(std::string name);
}

#include "Player.h"
#include "Level.h"

#endif /* defined(__Underneath__World__) */

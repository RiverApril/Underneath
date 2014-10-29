//
//  World.h
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__World__
#define __Underneath__World__

#include "Global.h"
#include "Level.h"
#include "Player.h"

class World {

public:

    World();

    ~World();

    Level* currentLevel;
    Player* currentPlayer;

};

namespace WorldLoader {
    bool exists(std::string name);
    bool load(std::string name);
    bool create(std::string name);
    extern World* loadedWorld;
}

#endif /* defined(__Underneath__World__) */

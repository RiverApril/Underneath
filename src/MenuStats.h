//
//  MenuStats.h
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuStats__
#define __Underneath__MenuStats__

#include "Menu.h"
#include "Alive.h"
#include "World.h"

namespace Ui {
    class MenuStats : public Menu {
    public:

        MenuStats(Player* player, World* w);

        ~MenuStats() {}

        void handleInput(int in);
        void update();

        Player* player;
        World* currentWorld;
        int scrollOffset = 0;
        int selected = 0;
        
    };
}

#endif /* defined(__Underneath__MenuStats__) */

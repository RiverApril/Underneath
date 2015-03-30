//
//  MenuChest.h
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuChest__
#define __Underneath__MenuChest__

#include "Menu.h"
#include "Alive.h"
#include "World.h"

namespace Ui {
    class MenuChest : public Menu {
    public:

        MenuChest(TEChest* chest, Player* user, World* w);

        ~MenuChest() {}

        void handleInput(int in);
        void update();

        TEChest* chest;
        Player* user;
        World* currentWorld;
        //int scrollOffset = 0;
        int selected = 0;
        bool selectedLeft = false;


        const int columnName = 1;
        const int columnQty = 39;
        //const int columnWeight = 35;
        //const int columnHitPoints = 35;
        const int columnInfo = 40;
        
    };
}

#endif /* defined(__Underneath__MenuChest__) */

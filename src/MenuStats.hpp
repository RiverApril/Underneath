//
//  MenuStats.h
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuStats__
#define __Underneath__MenuStats__

#include "Menu.hpp"
#include "Alive.hpp"
#include "World.hpp"

namespace Ui {

    class MenuStats : public Menu {
    public:

        MenuStats(Player* player, World* w);

        ~MenuStats() {
            delete shouldSpendPoint;
        }

        bool openUi();
        void handleInput(int in);
        void update();

        Player* player;
        World* currentWorld;
        int selected = 0;
        yesNo* shouldSpendPoint = new yesNo(aUndefined);

    };
}

#endif /* defined(__Underneath__MenuStats__) */

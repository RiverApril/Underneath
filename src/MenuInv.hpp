//
//  MenuInv.h
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuInv__
#define __Underneath__MenuInv__

#include "Menu.hpp"
#include "Player.hpp"
#include "World.hpp"

namespace Ui {

    class MenuInv : public Menu {
    public:

        MenuInv(Player* player, World* w, int* useItem);

        ~MenuInv() {
        }

        void handleInput(int in);
        void update();

        Player* player;
        World* currentWorld;
        //int scrollOffset = 0;
        int selected = 0;

        int* useItem;


        const int columnPrefixChar = 0;
        const int columnName = 2;
        const int columnQty = 39;
        //const int columnWeight = 35;
        //const int columnHitPoints = 35;
        const int columnInfo = 40;

    };
}

#endif /* defined(__Underneath__MenuInv__) */

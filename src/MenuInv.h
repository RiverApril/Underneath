//
//  MenuInv.h
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuInv__
#define __Underneath__MenuInv__

#include "Menu.h"
#include "Alive.h"
#include "World.h"

namespace Ui {
    class MenuInv : public Menu {
    public:

        MenuInv(Alive* alive, World* w, int* useItem);

        ~MenuInv() {}

        void handleInput(int in);
        void update();

        Alive* alive;
        World* currentWorld;
        int scrollOffset = 0;
        int selected = 0;

        int* useItem;


        const int columnPrefixChar = 0;
        const int columnName = 2;
        const int columnQty = 20;
        const int columnWeight = 25;
        const int columnHitPoints = 30;
        const int columnRange = 35;
        
    };
}

#endif /* defined(__Underneath__MenuInv__) */

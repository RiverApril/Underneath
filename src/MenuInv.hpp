//
//  MenuInv.hpp
//  Underneath
//
//  Created by Emily Atlee on 11/12/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__MenuInv__
#define __Underneath__MenuInv__

#include "Menu.hpp"
#include "EntityPlayer.hpp"
#include "World.hpp"

namespace Ui {

    class MenuInv : public Menu {
    public:

        MenuInv(World* w, int* useItem);

        ~MenuInv() {
        }

        void handleInput(int in);
        void update();

        
        World* currentWorld;
        //int scrollOffset = 0;
        int selected = 0;

        int* useItem;

        int flashImportantInfo = 0;


        const int columnPrefixChar = 0;
        const int columnName = 2;
        const int columnQty = 39;
        //const int columnWeight = 35;
        //const int columnHitPoints = 35;
        const int columnInfo = 40;

    };
}

#endif /* defined(__Underneath__MenuInv__) */

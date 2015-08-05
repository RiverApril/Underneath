//
//  MenuShop.h
//  Underneath
//
//  Created by Braeden Atlee on 8/5/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuShop__
#define __Underneath__MenuShop__

#include "Menu.hpp"
#include "EntityShop.hpp"
#include "World.hpp"

namespace Ui {

    class MenuShop : public Menu {
    public:

        MenuShop(EntityShop* shop, EntityPlayer* user, World* w);

        ~MenuShop() {
        }

        void handleInput(int in);
        void update();

        EntityShop* shop;
        EntityPlayer* user;
        World* currentWorld;
        int selected = 0;
        bool selectedLeft = false;


        const int columnName = 1;
        const int columnQty = 39;
        const int columnInfo = 40;

    };
}

#endif /* defined(__Underneath__MenuShop__) */

//
//  MenuUseInInv.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/4/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuUseInInv__
#define __Underneath__MenuUseInInv__

#include "Menu.hpp"
#include "EntityPlayer.hpp"
#include "World.hpp"

namespace Ui {

    class MenuUseInInv : public Menu {
    public:

        MenuUseInInv(EntityPlayer* player, World* w, int useItem);

        ~MenuUseInInv() {
        }

        void handleInput(int in);
        void update();

        EntityPlayer* player;
        World* currentWorld;
        int selected = 0;
        int useItem = -1;
        
    };
}

#endif /* MenuUseInInv_hpp */

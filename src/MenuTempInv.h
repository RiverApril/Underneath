//
//  MenuTempInv.h
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuTempInv__
#define __Underneath__MenuTempInv__

#include "Menu.h"
#include "Alive.h"
#include "World.h"

namespace Ui {
    class MenuTempInv : public Menu {
    public:

        MenuTempInv(Alive* alive, World* w);

        ~MenuTempInv() {}

        void handleInput(int in);
        void update();

        Alive* alive;
        World* currentWorld;
        int scrollOffset = 0;
        int selected = 0;
        
    };
}

#endif /* defined(__Underneath__MenuTempInv__) */

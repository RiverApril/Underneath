//
//  MenuTempInv.h
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuTempInv__
#define __Underneath__MenuTempInv__

#include "Ui.h"
#include "Alive.h"

namespace Ui {
    class MenuTempInv : public Ui::Menu {
    public:

        MenuTempInv(shared_ptr<Alive> alive);

        ~MenuTempInv() {}

        void handleInput(int in);
        void update();

        shared_ptr<Alive> alive;
        int scrollOffset = 0;
        int selected = 0;
        
    };
}

#endif /* defined(__Underneath__MenuTempInv__) */

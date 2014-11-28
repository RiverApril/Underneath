//
//  MenuTempMessage.h
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuTempMessage__
#define __Underneath__MenuTempMessage__

#include "Menu.h"

namespace Ui {
    class MenuTempMessage : public Menu {
    public:

        MenuTempMessage(string message);

        ~MenuTempMessage() {}

        void handleInput(int in);
        void update();

        string message;
        
        
    };
}

#endif /* defined(__Underneath__MenuTempMessage__) */

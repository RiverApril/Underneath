//
//  MenuMessage.h
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuMessage__
#define __Underneath__MenuMessage__

#include "Menu.h"

namespace Ui {
    class MenuMessage : public Menu {
    public:

        MenuMessage(string message);

        ~MenuMessage() {}

        void handleInput(int in);
        void update();

        string message;
        
        
    };
}

#endif /* defined(__Underneath__MenuMessage__) */

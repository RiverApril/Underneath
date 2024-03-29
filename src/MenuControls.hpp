//
//  MenuControls.hpp
//  Underneath
//
//  Created by Emily Atlee on 4/14/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__MenuControls__
#define __Underneath__MenuControls__

#include "Menu.hpp"

namespace Ui {

    class MenuControls : public Menu {
    public:

        MenuControls();

        ~MenuControls() {

        }
        
        void handleInput(int in);
        void update();

        int selected = 0;
        
    };
}

#endif /* defined(__Underneath__MenuControls__) */

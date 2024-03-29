//
//  MenuSettings.hpp
//  Underneath
//
//  Created by Emily Atlee on 7/19/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__MenuSettings__
#define __Underneath__MenuSettings__

#include "Menu.hpp"

namespace Ui {

    class MenuSettings : public Menu {
    public:

        MenuSettings();

        ~MenuSettings() {

        }

        void handleInput(int in);
        void update();
        bool openUi();

        int selected = 0;
        
        
    };
}

#endif /* defined(__Underneath__MenuSettings__) */

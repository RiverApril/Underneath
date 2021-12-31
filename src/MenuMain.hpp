//
//  MenuMain.hpp
//  Underneath
//
//  Created by Emily Atlee on 10/29/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuMain__
#define __Underneath__UiMenuMain__

#include "Menu.hpp"
#include "Art.hpp"

namespace Ui {

    class MenuMain : public Menu {
    public:

        MenuMain();

        ~MenuMain();


        bool openUi();
        void closeUi();
        void handleInput(int in);
        void update();

        int selection = 0;
    };
}

#endif /* defined(__Underneath__UiMenuMain__) */

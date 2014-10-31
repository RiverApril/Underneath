//
//  MenuMain.h
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuMain__
#define __Underneath__UiMenuMain__

#include "Ui.h"

namespace Ui {
    class MenuMain : public Ui::Menu {
    public:

        MenuMain() : Menu(100) {}

        ~MenuMain() {}

        void openUi();
        void handleInput(int in);
        void update();

        int selection = 0;
    };
}

#endif /* defined(__Underneath__UiMenuMain__) */

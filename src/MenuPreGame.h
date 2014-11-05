//
//  MenuPreGame.h
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuPreGame__
#define __Underneath__MenuPreGame__

#include "Ui.h"

namespace Ui {
    class MenuPreGame : public Ui::Menu {
    public:

        MenuPreGame() : Menu(100) {}

        ~MenuPreGame() {}

        void openUi();
        void handleInput(int in);
        void update();

        int selection = 0;
        std::string name = "";
    };
}

#endif /* defined(__Underneath__MenuPreGame__) */

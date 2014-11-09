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

        MenuPreGame() : Menu(false) {}

        ~MenuPreGame() {
            delete deleteAnswer;
        }

        void openUi(Menu* oldMenu);
        void closeUi(Menu* newMenu);
        void handleInput(int in);
        void update();

        int selection = 0;
        std::string name = "";

        bool* deleteAnswer;
    };
}

#endif /* defined(__Underneath__MenuPreGame__) */

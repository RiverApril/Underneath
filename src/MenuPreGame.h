//
//  MenuPreGame.h
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuPreGame__
#define __Underneath__MenuPreGame__

#include "Menu.h"

namespace Ui {
    class MenuPreGame : public Menu {
    public:

        MenuPreGame() : Menu() {}

        ~MenuPreGame() {
            delete deleteAnswer;
        }

        bool openUi();
        void closeUi();
        void handleInput(int in);
        void update();

        int selection = 0;
        string name = "";

        yesNo* deleteAnswer = new yesNo(aUndefined);

    private:
        static constexpr int selPlay = 0;
        static constexpr int selDel = 1;
        static constexpr int selBack = 2;

        static constexpr int maxUiSelection = selBack;
    };
}

#endif /* defined(__Underneath__MenuPreGame__) */

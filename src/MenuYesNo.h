//
//  MenuYesNo.h
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuYesNo__
#define __Underneath__MenuYesNo__

#include "Menu.h"

namespace Ui {

    class MenuYesNo : public Menu {
    public:

        MenuYesNo(string question, yesNo* answer, bool canEscape);

        ~MenuYesNo() {
        }

        void handleInput(int in);
        void update();

        string question;
        yesNo* answer;
        bool canEscape;


    };
}

#endif /* defined(__Underneath__MenuYesNo__) */

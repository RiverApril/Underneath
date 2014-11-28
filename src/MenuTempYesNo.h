//
//  MenuTempYesNo.h
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuTempYesNo__
#define __Underneath__MenuTempYesNo__

#include "Menu.h"

namespace Ui {
    class MenuTempYesNo : public Menu {
    public:

        MenuTempYesNo(string question, yesNo* answer, bool canEscape);

        ~MenuTempYesNo() {}

        void handleInput(int in);
        void update();

        string question;
        yesNo* answer;
        bool canEscape;


    };
}

#endif /* defined(__Underneath__MenuTempYesNo__) */

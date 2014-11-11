//
//  MenuTempYesNo.h
//  Underneath
//
//  Created by Braeden Atlee on 11/7/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuTempYesNo__
#define __Underneath__MenuTempYesNo__

#include "Ui.h"

namespace Ui {
    class MenuTempYesNo : public Ui::Menu {
    public:

        MenuTempYesNo(string question, bool* answer);

        ~MenuTempYesNo() {}

        void handleInput(int in);
        void update();

        string question;
        bool* answer;


    };
}

#endif /* defined(__Underneath__MenuTempYesNo__) */

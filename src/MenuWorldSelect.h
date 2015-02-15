//
//  MenuWorldSelect.h
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuWorldSelect__
#define __Underneath__MenuWorldSelect__

#include "Menu.h"

namespace Ui {
    class MenuWorldSelect : public Menu {
    public:

        MenuWorldSelect() : Menu() {}

        ~MenuWorldSelect() {
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
        static const int selPlay = 0;
		static const int selDel = 1;
		static const int selBack = 2;

		static const int maxUiSelection = selBack;
    };
}

#endif /* defined(__Underneath__MenuWorldSelect__) */

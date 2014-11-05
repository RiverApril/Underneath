//
//  MenuMain.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuMain.h"
#include "MenuGame.h"
#include "MenuPreGame.h"
#include "Global.h"

namespace Ui {

    void MenuMain::openUi() {

    }

    void MenuMain::handleInput(int in) {
        const int maxUiSelection = 1;

        move(selection+2, 0);
        clrtoeol();

        switch (in) {
            case KEY_ENTER:
            case 13:
            case '\n':
            case ' ':
                switch (selection) {
                    case 0:
                        changeMenu(new MenuPreGame());
                        break;

                    case maxUiSelection:
                        running = false;
                        break;

                    default:
                        break;
                }
                break;

            case KEY_UP:
                selection--;
                if(selection<0) {
                    selection = maxUiSelection;
                }
                break;

            case KEY_DOWN:
                selection++;
                if(selection>maxUiSelection) {
                    selection = 0;
                }
                break;

            default:
                break;
        }

    }

    void MenuMain::update() {
        if(limitedColorMode) {
            setColor(0);
            mvaddstr(0, 0, "Limited Color Mode");
        }
        setColor(C_WHITE);
        mvaddstr(1, 1, "Main Menu");
        mvaddstr(2, 3, "Play");
        mvaddstr(3, 3, "Exit");
        mvaddstr(selection+2, 1, "-");
        move(6, 0);
        for(int i=0; i<COLORS; i++) {
            attrset(COLOR_PAIR(i));
            printw("%X ", i);
        }
        refresh();
    }
    
}

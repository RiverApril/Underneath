//
//  MenuMain.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuMain.h"
#include "MenuGame.h"
#include "MenuWorldSelect.h"
#include "Global.h"

namespace Ui {

    bool MenuMain::openUi() {
        move(0, 0);
        clrtobot();
        return true;
    }

    void MenuMain::closeUi() {
        
    }

    void MenuMain::handleInput(int in) {
        const int maxUiSelection = 1;

        move(selection+2, 0);
        clrtoeol();

        switch (in) {
            case '\n':
            case ' ':
                switch (selection) {
                    case 0:
                        openMenu(new MenuWorldSelect());
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
        printCenter(2,  "  _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___  ");
        printCenter(3,  " | | |   |   \\ __|  _|   | __| _ |_ _| | | ");
        printCenter(4,  " | | | | | | | __| | | | | __|   || ||   | ");
        printCenter(5,  " |___|_|_|___/___|_| |_|_|___|_|_||_||_|_| ");
        printCenter(6,  " (    )     (    (       )        (      ) ");
        printCenter(7,  "  )  (       )    )     (          )    (  ");
        printCenter(8,  " (    )     (    (                (      ) ");
        printCenter(9,  "  )  (       )                     )       ");
        printCenter(10, " (          (                     (        ");
        printCenter(11, "  )                                )       ");
        printCenter(12, "                                  (        ");
        printCenter(10, "%sPlay%s", selection==0?"- ":"  ", selection==0?" -":"  ");
        printCenter(11, "%sExit%s", selection==1?"- ":"  ", selection==1?" -":"  ");
        /*move(10, 0);
        for(int i=0; i<COLORS; i++) {
            attrset(COLOR_PAIR(i));
            printw("%X ", i);
        }*/
        
        refresh();
    }
    
}

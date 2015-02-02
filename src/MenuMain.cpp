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

    MenuMain::MenuMain() : Menu(){}

    MenuMain::~MenuMain(){}

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
        
        int a = 2;
        
        setColor(C_WHITE);
        /*printCenter(a++, "  _ _ ___ ___ ___ ___ ___ ___ ___ ___ ___  ");
        printCenter(a++, " | | |   |   \\ __|  _|   | __| _ |_ _| | | ");
        printCenter(a++, " | | | | | | | __| | | | | __|   || ||   | ");
        printCenter(a++, " |___|_|_|___/___|_| |_|_|___|_|_||_||_|_| ");
        printCenter(a++, " (    )     (    (       )        (      ) ");
        printCenter(a++, "  )  (       )    )     (          )    (  ");
        printCenter(a++, " (    )     (    (                (      ) ");
        printCenter(a++, "  )  (       )                     )       ");
        printCenter(a++, " (          (                     (        ");
        printCenter(a++, "  )                                )       ");
        printCenter(a++, "                                  (        ");*/
        Arts::artList[Arts::artTitle]->printCenter(a);
        a+=Arts::artList[Arts::artTitle]->getSize().y;
        printCenter(a++, "%sPlay%s", selection==0?"- ":"  ", selection==0?" -":"  ");
        printCenter(a++, "%sExit%s", selection==1?"- ":"  ", selection==1?" -":"  ");
        
        a+=4;
        
        if(limitedColorMode) {
            mvaddstr(a, 0, "Limited Color Mode");
            
            swap(C_DARK_RED, C_DARK_BLUE);
            swap(C_LIGHT_RED, C_LIGHT_BLUE);
            swap(C_DARK_CYAN, C_DARK_YELLOW);
            swap(C_LIGHT_CYAN, C_LIGHT_YELLOW);
        }
        
        a++;

        setColor(C_DARK_BLACK);
        mvaddstr(a, 0, "D_BLACK");
        setColor(C_DARK_RED);
        mvaddstr(a, 8, "D_RED");
        setColor(C_DARK_GREEN);
        mvaddstr(a, 8+6, "D_GREEN");
        setColor(C_DARK_YELLOW);
        mvaddstr(a, 8+6+8, "D_YELLOW");
        setColor(C_DARK_BLUE);
        mvaddstr(a, 8+6+8+9, "D_BLUE");
        setColor(C_DARK_MAGENTA);
        mvaddstr(a, 8+6+8+9+7, "D_MAGENTA");
        setColor(C_DARK_CYAN);
        mvaddstr(a, 8+6+8+9+7+10, "D_CYAN");
        setColor(C_DARK_WHITE);
        mvaddstr(a, 8+6+8+9+7+10+7, "D_WHITE");

        a++;

        setColor(C_LIGHT_BLACK);
        mvaddstr(a, 0, "L_BLACK");
        setColor(C_LIGHT_RED);
        mvaddstr(a, 8, "L_RED");
        setColor(C_LIGHT_GREEN);
        mvaddstr(a, 8+6, "L_GREEN");
        setColor(C_LIGHT_YELLOW);
        mvaddstr(a, 8+6+8, "L_YELLOW");
        setColor(C_LIGHT_BLUE);
        mvaddstr(a, 8+6+8+9, "L_BLUE");
        setColor(C_LIGHT_MAGENTA);
        mvaddstr(a, 8+6+8+9+7, "L_MAGENTA");
        setColor(C_LIGHT_CYAN);
        mvaddstr(a, 8+6+8+9+7+10, "L_CYAN");
        setColor(C_LIGHT_WHITE);
        mvaddstr(a, 8+6+8+9+7+10+7, "L_WHITE");
        
        refresh();
    }
    
}

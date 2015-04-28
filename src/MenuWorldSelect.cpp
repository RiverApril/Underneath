//
//  MenuWorldSelect.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/3/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuWorldSelect.h"
#include "MenuMain.h"
#include "MenuGame.h"
#include "MenuYesNo.h"
#include "MenuPlayerSetup.h"
#include "Controls.h"

namespace Ui {

    bool MenuWorldSelect::openUi() {
        if (*deleteAnswer == aYes) {
            WorldLoader::deleteWorld(name);
            *deleteAnswer = aUndefined;
        }
        move(0, 0);
        clrtobot();
        selection = selPlay;
        return true;
    }

    void MenuWorldSelect::closeUi() {

    }

    void MenuWorldSelect::handleInput(int in) {

        move(selection + 2, 0);
        clrtoeol();

        if(in == '\n'){
            switch (selection) {
                case selPlay:
                    if (name.length() > 0) {
                        if (WorldLoader::exists(name)) {
                            openMenu(new MenuGame(name, Abilities<int>()));
                        } else {
                            openMenu(new MenuGame(name, Abilities<int>()));
                        }
                    }
                    break;

                case selDel:
                    if (WorldLoader::exists(name)) {
                        openMenu(new MenuYesNo("Are you sure you want to delete '" + name + "' ?", deleteAnswer, true));
                    }
                    break;

                case selBack:
                    closeThisMenu();
                    break;

                default:
                    break;
            }
        }else if(in == Key::uiUp){
            selection--;
            if (selection < 0) {
                selection = maxUiSelection;
            }
        }else if(in == Key::uiDown){
            selection++;
            if (selection > maxUiSelection) {
                selection = 0;
            }
        }else if(in == KEY_ESCAPE){
            closeThisMenu();
        }else if(in == KEY_BACKSPACE || in == 8 || in == 127){
            if (name.length() > 0) {
                name = name.substr(0, name.length() - 1);
            }
        }else if ((in == '_') || (in >= '0' && in <= '9') || (in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
                    name += (char) in;
        }

    }

    void MenuWorldSelect::update() {

        bool e = WorldLoader::exists(name);

        setColor(C_WHITE);

        printCenter(2, "Enter Name");

        printCenter(5, e ? "  %sContinue%s  " : "%sCreate New%s",
                selection == 0 ? "- " : "  ", selection == 0 ? " -" : "  ");

        setColor(e ? C_LIGHT_RED : C_DARK_GRAY);
        printCenter(6, "%sDelete%s",
                selection == 1 ? "- " : "  ", selection == 1 ? " -" : "  ");
        setColor(C_WHITE);

        printCenter(8, "%sBack%s", selection == 2 ? "- " : "  ",
                selection == 2 ? " -" : "  ");


        setColor(e ? C_LIGHT_GREEN : C_LIGHT_YELLOW);
        move(3, 0);
        clrtoeol();
        printCenter(3, name);
        setColor(C_WHITE, C_BLACK, A_BLINK);
        printCenterOffset(3, ((int) name.length() / 2) + 1, "_");

        refresh();
    }

}

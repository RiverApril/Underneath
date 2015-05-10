//
//  MenuPlayerSetup.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuPlayerSetup.hpp"
#include "MenuMain.hpp"
#include "MenuGame.hpp"
#include "MenuYesNo.hpp"
#include "Player.hpp"

namespace Ui {

    bool MenuPlayerSetup::openUi() {
        move(0, 0);
        clrtobot();
        selection = 0;
        return true;
    }

    void MenuPlayerSetup::closeUi() {

    }

    void MenuPlayerSetup::handleInput(int in) {

        switch (in) {
            case '\n':
                switch (selection) {

                    case selBack:
                        closeThisMenu();
                        break;

                    default:
                        if (name.length() > 0 && remainingPoints == 0) {
                            openMenu(new MenuGame(name, Abilities<int>(addOne(abilityPoints))));
                        }
                        break;
                }
                break;

            case KEY_UP:
                selection--;
                if (selection < 0) {
                    selection = maxUiSelection;
                }
                break;

            case KEY_DOWN:
                selection++;
                if (selection > maxUiSelection) {
                    selection = 0;
                }
                break;

            case KEY_LEFT:
                if (selection < abilityCount) {
                    if (abilityPoints[selection] > 0) {
                        abilityPoints[selection]--;
                        remainingPoints++;
                    }
                }
                break;
            case KEY_RIGHT:
                if (selection < abilityCount) {
                    if (remainingPoints > 0) {
                        if (abilityPoints[selection] < pointMax) {
                            abilityPoints[selection]++;
                            remainingPoints--;
                        }
                    }
                }
                break;

            case 27:
                closeThisMenu();
                break;
        }

    }

    void MenuPlayerSetup::renderAbility(int y, int xOffset, int sel, int points, string name) {

        setColor((selection == sel) ? C_BLACK : C_WHITE, (selection == sel) ? C_WHITE : C_BLACK);
        printCenterOffset(y, xOffset, name + "  |");
        for (int i = 1; i <= pointMax; i++) {
            addch((i <= points) ? '|' : '.');
        }
    }

    void MenuPlayerSetup::update() {

        refresh();

        setColor(C_WHITE);

        printCenter(2, "Select Abilities");


        printCenter(5, "Remaining");
        printCenterOffset(6, -(totalPoints / 2), "");
        for (int i = 1; i <= totalPoints; i++) {
            addch((i <= remainingPoints) ? '|' : '-');
        }

        for (int i = 0; i < abilityCount; i++) {
            renderAbility(8 + i, -(pointMax / 2), i, abilityPoints[i], abilityNamesRightAligned[i]);
        }


        setColor(C_WHITE, C_BLACK);

        printCenter(16, "%sContinue%s", selection == selContinue ? "- " : "  ",
                selection == selContinue ? " -" : "  ");

        printCenter(17, "%sBack%s", selection == selBack ? "- " : "  ",
                selection == selBack ? " -" : "  ");

        if (selection < abilityCount) {
            printCenter(19, abilityInfo[selection]);
        }

        refresh();
    }

}

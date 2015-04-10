//
//  MenuStats.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuStats.h"
#include "Math.h"
#include "ItemEntity.h"
#include "Controls.h"
#include "Utility.h"
#include "MenuYesNo.h"
#include "MenuMessage.h"

namespace Ui {

    bool MenuStats::openUi() {
        if (*shouldSpendPoint == aYes) {

            player->abilityPoints--;
            player->abilities.list[selected]++;
            player->updateVariablesForAbilities();

            *shouldSpendPoint = aUndefined;
        }
        return true;
    }

    MenuStats::MenuStats(Player* player, World* w) : Menu() {
        this->player = player;
        this->currentWorld = w;
    }

    void MenuStats::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;

        } else if (in == Key::uiDown) {
            selected++;

        } else if (in == '\n' || in == ' ') {
            if (player->abilityPoints > 0) {
                if (player->abilities[selected] < player->maxAbilities[selected]) {
                    openMenu(new MenuYesNo(formatString("Are you sure you want to spend a skill point on %s?", abilityNames[selected].c_str()), shouldSpendPoint, true));
                } else {
                    openMenu(new MenuMessage("Level Maxed."));
                }
            }
        } else if (in == KEY_ESCAPE || in == Key::statsMenu) {
            closeThisMenu();

        }
        if (selected < 0) {
            selected = 0;
        }
        if (selected >= abilityCount) {
            selected = abilityCount - 1;
        }
    }

    void MenuStats::update() {
        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        int a = 2;
        printCenter(a++, "Level: %d", player->level);
        printCenter(a++, "XP: %d / %d", (int) player->xp, (int) player->nextLevelXp);
        printCenter(a++, "%s", Utility::makeBar((int) player->xp, (int) player->nextLevelXp, 20).c_str());
        if (player->abilityPoints > 0) {
            printCenter(a++, "You have skill points to spend: %d", player->abilityPoints);
        } else {
            a++;
        }

        a++;

        for (int i = 0; i < abilityCount; i++) {
            if (i == selected) {
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(a++, 3, "%s: %3d   %s", abilityNamesRightAligned[i].c_str(), player->abilities[i], abilityInfo[i].c_str());
            if (i == selected) {
                setColor(C_WHITE);
            }
        }

    }
}

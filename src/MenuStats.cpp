//
//  MenuStats.cpp
//  Underneath
//
//  Created by Emily Atlee on 12/27/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#include "MenuStats.hpp"
#include "Math.hpp"
#include "EntityItem.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "MenuYesNo.hpp"
#include "MenuMessage.hpp"

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

    MenuStats::MenuStats(EntityPlayer* player, World* w) : Menu() {
        this->player = player;
        this->currentWorld = w;
    }

    void MenuStats::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;

        } else if (in == Key::uiDown) {
            selected++;

        } else if (in == '\n') {
            if (player->abilityPoints > 0) {
                if (player->abilities[selected] < player->maxAbilities[selected]-1) {
                    //openMenu(new MenuYesNo(formatString("Are you sure you want to spend a skill point on %s?", abilityNames[selected].c_str()), shouldSpendPoint, true));

                    player->abilityPoints--;
                    player->abilities.list[selected]++;
                    player->updateVariablesForAbilities();

                } else {
                    openMenu(new MenuMessage("Level Maxed."));
                }
            }
        } else if (in == KEY_ESCAPE || in == Key::statsMenu) {
            closeThisMenu();
            return;
        }
    }

    void MenuStats::update() {

        if (selected < 0) {
        	selected = abilityCount - 1;
    	}
        if (selected >= abilityCount) {
            selected = 0;
        }
        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        int a = 2;
        printCenter(a++, "Level: %d", player->level);
        printCenter(a++, "XP: %d / %d", (int) player->xp, (int) player->nextLevelXp);
        printCenter(a++, "%s", Utility::makeBar((int) player->xp, (int) player->nextLevelXp, terminalSize.x-4).c_str());
        if (player->abilityPoints > 0) {
            printCenter(a++, "You have skill points to spend: %d", player->abilityPoints);
        } else {
            a++;
        }

        a++;
        int b = a;

        mvprintw(b++, 1, "Abilities:");

        for (int i = 0; i < abilityCount; i++) {
            if (i == selected) {
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(b++, 3, "%s: %d", abilityNamesRightAligned[i].c_str(), player->abilities[i]);
            if (i == selected) {
                setColor(C_WHITE);
            }
        }

        b = a;

        mvprintw(b++, terminalSize.x/3, "Values:");
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %d%%", "Melee Attack", (int)(player->strMult*100));
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %d%%", "Range Attack", (int)(player->dexMult*100));
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %d%%", "Magic Attack", (int)(player->intMult*100));
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %d%%", "Dodge Chance", 100-(int)(player->dodgeChance*100));
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %.2f", "Interact Delay", player->interactDelay);
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %.2f", "Move Delay", player->moveDelay);
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %.2f", "Heal Mult.", player->healBase);
        mvprintw(b++, terminalSize.x/3 + 2, "%s: %.2f", "Mana Heal Mult.", player->healManaBase);

        b = a;

        mvprintw(b++, terminalSize.x/3*2, "Defenses:");

        for (DamageType d = damNone; d<damQty;d = (DamageType)((int)d+1)) {
            double dm = 1.0 - player->getRecivedDamageMultiplierFromArmorAndEquips(d);
            if(dm > 0){
                mvprintw(b++, terminalSize.x/3*2 + 2, "%s: %d%%", damageTypeName(d).c_str(), (int)(dm*100));
            }
        }

        printCenter(terminalSize.y - 2, abilityInfo[selected]);

    }
}

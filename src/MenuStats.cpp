//
//  MenuStats.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
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
                if (player->abilities[selected] < player->maxAbilities[selected]) {
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
        printCenter(a++, "%s", Utility::makeBar((int) player->xp, (int) player->nextLevelXp, terminalSize.x-4).c_str());
        if (player->abilityPoints > 0) {
            printCenter(a++, "You have skill points to spend: %d", player->abilityPoints);
        } else {
            a++;
        }

        a++;
        int b = a;

        mvprintw(a++, 1, "Abilities:");

        for (int i = 0; i < abilityCount; i++) {
            if (i == selected) {
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(a++, 3, "%s: %d", abilityNamesRightAligned[i].c_str(), player->abilities[i]);
            if (i == selected) {
                setColor(C_WHITE);
            }
        }

        mvprintw(b++, terminalSize.x/2 + 3, "Defenses:");
        

        map<DamageType, double> calculatedDefenses;

        for(pair<EquipSlot, ItemEquipable*> p : player->equipedItems){
            if(p.second){
                ItemArmor* armor = dynamic_cast<ItemArmor*>(p.second);
                if(armor){
                    for(Defense d : armor->defenses){
                        if(calculatedDefenses.count(d.damageType)){
                            calculatedDefenses[d.damageType] += d.amount;
                        }else{
                            calculatedDefenses[d.damageType] = d.amount;
                        }
                    }
                }
            }
        }

        for (pair<DamageType, double> d : calculatedDefenses) {
            mvprintw(b++, terminalSize.x/2 + 5, "%s: %d%%", damageTypeName(d.first).c_str(), (int)(d.second*100.0));
        }

        printCenter(terminalSize.y - 2, abilityInfo[selected]);

    }
}

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

namespace Ui {

    MenuStats::MenuStats(Player* player, World* w) : Menu(){
        this->player = player;
        this->currentWorld = w;
    }

    void MenuStats::handleInput(int in){
        switch (in) {
            case KEY_UP:
                selected--;
                break;

            case KEY_DOWN:
                selected++;
                break;

            case KEY_ESCAPE:
            case Key::stats:
                closeThisMenu();
                break;
        }
        if(selected<0){
            selected = 0;
        }
        if(selected >= player->inventory.size()){
            selected = (int)player->inventory.size()-1;
        }
    }

    void MenuStats::update() {
        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        mvprintw(0, 0, "Abilities: ");
        for(int i=0;i<abilityCount;i++){
            mvprintw(i+2, 1, "%s: %2d  xp[%s] (%.0f/%.0f)",
                     string(abilityNamesRightAligned[i]).c_str(),
                     player->levels.list[i],
                     StringUtility::makeBar((player->xp.list[i]), (player->nextLevelXp.list[i]), 5, '=', ' ').c_str(),
                     (player->xp.list[i]),
                     (player->nextLevelXp.list[i]));
            ;
        }
    }
}


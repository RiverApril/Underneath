//
//  MenuInv.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuInv.h"
#include "Math.h"
#include "ItemEntity.h"
#include "Controls.h"
#include "Utility.h"

namespace Ui {

    MenuInv::MenuInv(Alive* alive, World* w, int* useItem) : Menu(){
        this->alive = alive;
        this->currentWorld = w;
        this->useItem = useItem;
        *useItem = -1;
    }

    void MenuInv::handleInput(int in){
        switch (in) {
            case KEY_UP:
                selected--;
                break;

            case KEY_DOWN:
                selected++;
                break;

            case 'd':{
                if(alive->inventory.size() > 0 && selected < alive->inventory.size()){
                    Item* drop = alive->inventory[selected];
                    if(alive->inventory[selected]->qty == 1){
                    	alive->removeItem(alive->inventory[selected], false);
                    }else{
                        alive->inventory[selected]->qty -= 1;
                    }
                    currentWorld->currentLevel->newEntity(new ItemEntity(drop, alive->pos));
                }
                break;
            }

            case 'e':{
                Weapon* weapon = dynamic_cast<Weapon*>(alive->inventory[selected]);
                if(weapon != nullptr){
                    if(alive->getActiveWeapon() == weapon){
                        alive->setActiveWeapon(nullptr);
                    }else{
                        alive->setActiveWeapon(weapon);
                    }
                }
                break;
            }

            case 'u':{
                *useItem = selected;
                closeThisMenu();
                break;
            }

            case KEY_ESCAPE:
            case Key::inventory:
                closeThisMenu();
                break;
        }
        if(selected<0){
            selected = 0;
        }
        if(selected >= alive->inventory.size()){
            selected = (int)alive->inventory.size()-1;
        }
    }


    void MenuInv::update() {
        setColor(C_WHITE);
        int minI = Math::max(0, scrollOffset);
        int maxI = (int)alive->inventory.size() - scrollOffset;
        move(0, 0);
        clrtobot();
        int totalWeight = 0;
        forVector(alive->inventory, i){
            totalWeight += alive->inventory[i]->weight;
        }
        mvprintw(0, 0, "Inventory   Total Weight: %-3d", totalWeight);

        setColor(C_BLACK, C_WHITE);
		mvhline(selected+3, 0, ' ', terminalSize.x);
        setColor(C_WHITE, C_BLACK);

        mvprintw(1, columnPrefixChar, "");mvvline(1, columnPrefixChar-1, '|', terminalSize.y);
        mvprintw(1, columnName, "Item");mvvline(1, columnName-1, '|', terminalSize.y);
        mvprintw(1, columnQty, "Qty");mvvline(1, columnQty-1, '|', terminalSize.y);
        mvprintw(1, columnWeight, "Wgt");mvvline(1, columnWeight-1, '|', terminalSize.y);
        mvprintw(1, columnHitPoints, "Hp");mvvline(1, columnHitPoints-1, '|', terminalSize.y);
        mvprintw(1, columnRange, "Rng");mvvline(1, columnRange-1, '|', terminalSize.y);

        mvhline(2, 0, '-', terminalSize.x);
        Item* item;
        Weapon* weapon;
        Ranged* ranged;
        int y = 3;
        for(int i=minI;i<maxI;i++){
            item = alive->inventory[i];
            weapon = dynamic_cast<Weapon*>(item);
            ranged = dynamic_cast<Ranged*>(item);
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(y, columnPrefixChar, "%c", item == alive->getActiveWeapon()?'E':' ');
            mvprintw(y, columnName, item->name.c_str());
            mvprintw(y, columnQty, "%4d", item->qty);
            mvprintw(y, columnWeight, "%2.1f", item->weight);
            if(weapon != nullptr){
                mvprintw(y, columnHitPoints, "%4d", weapon->baseDamage);
            }
            if(ranged != nullptr){
                mvprintw(y, columnRange, "%4d", ranged->range);
            }
            if(i == selected){
                setColor(C_WHITE, C_BLACK);
            }
            y++;
        }
    }
}


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
                    alive->removeItem(alive->inventory[selected], false);
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
        mvprintw(0, 0, (alive->getName()+"'s Inventory: ").c_str());
        mvhline(1, 0, '-', terminalSize.x);
        for(int i=minI;i<maxI;i++){
            char prefix = ' ';
            if(alive->inventory[i] == alive->getActiveWeapon()){
                prefix = 'E';
            }
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(i+2, 0, ("%c %s"), prefix, alive->inventory[i]->name.c_str());
            if(i == selected){
                setColor(C_WHITE, C_BLACK);
            }
        }
    }
}


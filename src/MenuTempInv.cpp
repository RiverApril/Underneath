//
//  MenuTempInv.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuTempInv.h"
#include "Math.h"

namespace Ui {

    MenuTempInv::MenuTempInv(shared_ptr<Alive> alive) : Menu(true){
        this->alive = alive;
    }

    void MenuTempInv::handleInput(int in){
        switch (in) {
            case KEY_UP:
                selected--;
                break;

            case KEY_DOWN:
                selected++;
                break;

            case 27: //Escape
            case 'i':
                changeMenu(parentMenu);
                break;
        }
        if(selected<0){
            selected = 0;
        }
        if(selected >= alive->inventory.size()){
            selected = (int)alive->inventory.size();
        }
    }

    void MenuTempInv::update(){
        int minI = Math::max(0, scrollOffset);
        int maxI = (int)alive->inventory.size() - scrollOffset;
        mvprintw(0, 0, (alive->getName()+"'s Inventory: ").c_str());
        mvhline(1, 0, '-', terminalSize.x);
        for(int i=minI;i<maxI;i++){
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(i+2, 0, ("- %s"), alive->inventory[i]->name.c_str());
            if(i == selected){
                setColor(C_WHITE, C_BLACK);
            }
        }
    }
}


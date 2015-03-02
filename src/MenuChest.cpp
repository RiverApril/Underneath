//
//  MenuChest.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuChest.h"
#include "Math.h"
#include "ItemEntity.h"
#include "Controls.h"
#include "Utility.h"
#include "Art.h"

namespace Ui {

    MenuChest::MenuChest(TEChest* chest, Alive* user, World* w) : Menu(){
        this->chest = chest;
        this->user = user;
        this->currentWorld = w;
    }

    void MenuChest::handleInput(int in){
        switch (in) {
            case KEY_UP:
                selected--;
                break;

            case KEY_DOWN:
                selected++;
                break;

            case 't':
            case 'e':{
                if(chest->inventory.size() > 0 && selected < chest->inventory.size()){
                    Item* take;
                    if(chest->inventory[selected]->qty == 1){
                        take = chest->inventory[selected];
                        chest->removeItem(chest->inventory[selected], false);
                    }else{
                        chest->inventory[selected]->qty -= 1;
                        take = Item::clone(chest->inventory[selected]);
                        take->qty = 1;
                    }
                    user->addItem(take);

                }
                break;
            }

            case 'T':
            case 'E':{
                if(chest->inventory.size() > 0 && selected < chest->inventory.size()){
                    Item* take;
                    take = chest->inventory[selected];
                    chest->removeItem(chest->inventory[selected], false);

                    user->addItem(take);
                }
                break;
            }

            case 'R':{
                if(chest->inventory.size() > 0){
                    user->addItems(chest->inventory);
                    chest->removeAllItems(false);
                }
            }

            case KEY_ESCAPE:
            case Key::inventory:
                closeThisMenu();
				return;
        }
        if(selected<0){
            selected = 0;
        }
        if(selected >= chest->inventory.size()){
            selected = (int)chest->inventory.size()-1;
        }
    }

    void MenuChest::update() {

        Ui::drawInventory(chest, selected, scrollOffset, "Chest");
        
    }
}

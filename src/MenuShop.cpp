//
//  MenuShop.cpp
//  Underneath
//
//  Created by Braeden Atlee on 8/5/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuShop.hpp"
#include "Math.hpp"
#include "EntityItem.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "Art.hpp"

#define from (selectedLeft?(Inventory*)user:(Inventory*)shop)
#define to (selectedLeft?(Inventory*)shop:(Inventory*)user)

namespace Ui {

    MenuShop::MenuShop(EntityShop* shop, EntityPlayer* user, World* w) : Menu() {
        this->shop = shop;
        this->user = user;
        this->currentWorld = w;
    }

    void MenuShop::handleInput(int in) {
        if (in == Key::uiLeft || in == Key::uiRight) {
            selectedLeft = !selectedLeft;

        } else if (in == Key::uiUp) {
            selected--;

        } else if (in == Key::uiDown) {
            selected++;

        } else if (in == Key::equip) {
            if (selectedLeft) {
                ItemEquipable* equipable = dynamic_cast<ItemEquipable*> (user->inventory[selected]);
                if (equipable) {
                    EquipSlot slot = user->getSlot(equipable);
                    if(slot){
                        user->equipItem(nullptr, slot);
                    }else{
                        user->equipItem(equipable);
                    }
                }
            } else {
                MenuShop::handleInput(Key::take);
            }

        } else if (in == Key::equipPrimary) {
            ItemEquipable* equipable = dynamic_cast<ItemEquipable*> (user->inventory[selected]);
            if (equipable) {
                EquipSlot slot = user->getSlot(equipable);
                if(slot){
                    user->equipItem(nullptr, slot);
                }else{
                    user->equipItem(equipable, equipable->getViableSlots()[0]);
                }
            }
        } else if (in == Key::take) {
            if (from->inventory.size() > 0 && selected < from->inventory.size()) {
                Item* take;
                if (from->inventory[selected]->qty == 1) {
                    take = from->inventory[selected];
                    from->removeItem(from->inventory[selected], false);
                } else {
                    from->inventory[selected]->qty -= 1;
                    take = Item::clone(from->inventory[selected]);
                    take->qty = 1;
                }
                to->addItem(take);

            }

        } else if (in == Key::takeStack) {
            if (from->inventory.size() > 0 && selected < from->inventory.size()) {
                Item* take;
                take = from->inventory[selected];
                from->removeItem(from->inventory[selected], false);

                to->addItem(take);
            }

        } else if (in == Key::takeAll) {
            if (from->inventory.size() > 0) {
                to->addItems(from->inventory);
                from->removeAllItems(false);
            }

        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        }
    }

    void MenuShop::update() {

        if (selected < 0) {
            selected = (int) from->inventory.size() - 1;
        }
        if (selected >= from->inventory.size()) {
            selected = 0;
        }

        Ui::drawInventory(currentWorld, user, selected, shop, "Shop", selectedLeft);

    }
}

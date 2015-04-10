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

#define from (selectedLeft?(Inventory*)user:(Inventory*)chest)
#define to (selectedLeft?(Inventory*)chest:(Inventory*)user)

namespace Ui {

    MenuChest::MenuChest(TEChest* chest, Player* user, World* w) : Menu() {
        this->chest = chest;
        this->user = user;
        this->currentWorld = w;
    }

    void MenuChest::handleInput(int in) {
        if (in == Key::uiLeft || in == Key::uiRight) {
            selectedLeft = !selectedLeft;

        } else if (in == Key::uiUp) {
            selected--;

        } else if (in == Key::uiDown) {
            selected++;

        } else if (in == Key::equip) {
            if (selectedLeft) {
                Weapon* weapon = dynamic_cast<Weapon*> (user->inventory[selected]);
                if (weapon) {
                    if (user->getActiveWeapon() == weapon) {
                        user->setActiveWeapon(nullptr);
                    } else {
                        user->setActiveWeapon(weapon);
                    }
                }
            } else {
                MenuChest::handleInput(Key::take);
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

        if (selected < 0) {
            selected = 0;
        }
        if (selected >= from->inventory.size()) {
            selected = (int) from->inventory.size() - 1;
        }
    }

    void MenuChest::update() {

        Ui::drawInventory(user, selected/*, scrollOffset*/, chest, "Chest", selectedLeft);

    }
}

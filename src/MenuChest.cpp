//
//  MenuChest.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuChest.hpp"
#include "Math.hpp"
#include "EntityItem.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "Art.hpp"

#define from (selectedLeft?(Inventory*)user:(Inventory*)chest)
#define to (selectedLeft?(Inventory*)chest:(Inventory*)user)

namespace Ui {

    MenuChest::MenuChest(TEChest* chest, EntityPlayer* user, World* w) : Menu() {
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

        } else if (in == Key::take) {
            if ((int)from->inventory.size() > 0 && selected < (int)from->inventory.size()) {
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
            if ((int)from->inventory.size() > 0 && selected < (int)from->inventory.size()) {
                Item* take;
                take = from->inventory[selected];
                from->removeItem(from->inventory[selected], false);

                to->addItem(take);
            }

        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        }
    }

    void MenuChest::update() {

        if (selected < 0) {
            selected = (int) from->inventory.size() - 1;
        }
        if (selected >= (int)from->inventory.size()) {
            selected = 0;
        }

        Ui::drawInventory(currentWorld, user, selected/*, scrollOffset*/, chest, formatString("%s's Inventory", user->getName().c_str()), "Chest", selectedLeft);

    }
}

//
//  MenuUseInInv.cpp
//  Underneath
//
//  Created by Emily Atlee on 12/4/15.
//  Copyright © 2015 Emily Atlee. All rights reserved.
//

#include "MenuUseInInv.hpp"
#include "Math.hpp"
#include "EntityItem.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "Art.hpp"

namespace Ui {

    MenuUseInInv::MenuUseInInv(EntityPlayer* player, World* w, int useItem) : Menu() {
        this->player = player;
        this->currentWorld = w;
        this->useItem = useItem;
        this->selected = useItem;
    }

    void MenuUseInInv::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;
        } else if (in == Key::uiDown) {
            selected++;
        } else if (in == Key::interact) {
            player->useItemOnOther(player->inventory[useItem], player->inventory[selected]);
            closeThisMenu();
            return;
        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        }
    }

    void MenuUseInInv::update() {
        if (selected < 0) {
            selected = (int) player->inventory.size() - 1;
        }
        if (selected >= (int) player->inventory.size()) {
            selected = 0;
        }

        Ui::drawInventory(currentWorld, player, selected, nullptr, "", "", true, false, false, useItem);
    }

}

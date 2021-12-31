//
//  MenuShop.cpp
//  Underneath
//
//  Created by Emily Atlee on 8/5/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
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

    bool MenuShop::openUi(){
        sort(shop->inventory.begin(), shop->inventory.end(), comparePrice);
        return true;
    }

    void MenuShop::handleInput(int in) {
        if (in == Key::uiLeft || in == Key::uiRight) {
            selectedLeft = !selectedLeft;

        } else if (in == Key::uiUp) {
            selected--;

        } else if (in == Key::uiDown) {
            selected++;

        } else if (in == Key::take) {
            if ((int)from->inventory.size() > 0 && selected < (int)from->inventory.size()) {
                if(to->getWallet() >= from->inventory[selected]->coinValue && from->inventory[selected]->coinValue >= 0){
                    debugf("Coin Value: %d   Wallet: %d", from->inventory[selected]->coinValue, to->getWallet());
                    Item* take;
                    if (from->inventory[selected]->qty == 1) {
                        take = from->inventory[selected];
                        from->removeItem(from->inventory[selected], false);
                    } else {
                        from->inventory[selected]->qty -= 1;
                        take = Item::clone(from->inventory[selected]);
                        take->qty = 1;
                    }
                    int coinValue = take->coinValue;
                    to->addItem(take); //Changes coin value
                    to->addToWallet(-coinValue);
                    from->addToWallet(coinValue);
                }

            }

        } else if (in == Key::takeStack) {
            if ((int)from->inventory.size() > 0 && selected < (int)from->inventory.size()) {
                if(to->getWallet() >= (from->inventory[selected]->coinValue * from->inventory[selected]->qty) && from->inventory[selected]->coinValue >= 0){
                    Item* take;
                    take = from->inventory[selected];
                    from->removeItem(from->inventory[selected], false);

                    int stackCoinValue = (take->coinValue * from->inventory[selected]->qty);
                    to->addItem(take); //Changes coin value
                    to->addToWallet(-stackCoinValue);
                    from->addToWallet(stackCoinValue);
                }
            }

        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        } else if (in == Key::sortInv){
            sort(from->inventory.begin(), from->inventory.end(), comparePointer<Item>);
            return;
        }
    }

    void MenuShop::update() {

        if (selected < 0) {
            selected = (int) from->inventory.size() - 1;
        }
        if (selected >= (int)from->inventory.size()) {
            selected = 0;
        }

        Ui::drawInventory(currentWorld, user, selected, shop, formatString("%s's Wallet: %d%s", user->getName().c_str(), user->getWallet(), SYMBOL_COIN), formatString("Shop Keeper's Wallet: %d%s", shop->getWallet(), SYMBOL_COIN), selectedLeft, true);

    }
}










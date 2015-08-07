//
//  Inventory.h
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Inventory__
#define __Underneath__Inventory__

#include "ItemWeapon.hpp"

struct Inventory {

    Inventory() {

    }

    virtual ~Inventory() {
        removeAllItems(true);
    }

    virtual void removeAllItems(bool deleteItem) {
        while (inventory.size() > 0) {
            Item* item = inventory[0];
            inventory.erase(inventory.begin());
            if (deleteItem) {
                delete item;
            }
        }
    }

    virtual bool removeItem(Item* item, bool deleteItem) {

        forVector(inventory, i) {
            Item* ie = inventory[i];
            if (ie == item) {
                inventory.erase(inventory.begin()+(long) i);
                if (deleteItem) {
                    delete item;
                }
                return true;
            }
        }
        return false;
    }

    virtual bool containsItem(Item* item) {

        forVector(inventory, i) {
            Item* ie = inventory[i];
            if (ie == item) {
                return true;
            }
        }
        return false;
    }

    virtual bool containsItemEqualingExceptQty(Item* item, int remove = 0){

        forVector(inventory, i) {
            Item* ie = inventory[i];
            if (ie->equalsExceptQty(item)) {
                if(ie->qty - remove >= 0){
                    ie->qty -= remove;
                    if(ie->qty == 0){
                        inventory.erase(inventory.begin()+(long) i);
                    }
                    return true;
                }
            }
        }
        return false;

    }

    virtual void addItems(vector<Item*> items) {
        for (Item* i : items) {
            addItem(i);
        }
    }

    virtual bool addItem(Item* newItem, int qty = -1);

    virtual int getCoinValue(Item* item);

    virtual int getWallet();

    virtual void addToWallet(int amount);

    vector<Item*> inventory;

};

#endif /* defined(__Underneath__Inventory__) */

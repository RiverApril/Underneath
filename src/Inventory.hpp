//
//  Inventory.hpp
//  Underneath
//
//  Created by Emily Atlee on 2/26/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Inventory__
#define __Underneath__Inventory__

#include "ItemWeapon.hpp"

class Inventory {
public:

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

    //return qty removed
    virtual int removeItemQty(int q, Item* item, bool deleteItem){
        forVector(inventory, i) {
            Item* ie = inventory[i];
            if (ie == item) {
                if(ie->qty > q){
                    ie->qty -= q;
                    return q;
                }else if(ie->qty == q){
                    return removeItem(item, deleteItem) ? q : 0;
                }else{
                    int qq = ie->qty;
                    return removeItem(item, deleteItem) ? qq : 0;
                }
            }
        }
        return 0;
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

    virtual bool addItem(Item* newItem);

    virtual int getCoinValue(Item* item);

    virtual int getWallet();

    virtual void addToWallet(int amount);

    vector<Item*> inventory;

};

#endif /* defined(__Underneath__Inventory__) */

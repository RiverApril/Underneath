//
//  Inventory.h
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Inventory__
#define __Underneath__Inventory__

#include "Weapon.h"

const int columnPrefixChar = 0;
const int columnName = 1;
const int columnQty = 39;
const int columnInfo = 40;

struct Inventory{

    Inventory(){

    }

    virtual ~Inventory(){
        
    }

    void removeAllItems(bool deleteItem){
        while(inventory.size() > 0){
            Item* item = inventory[0];
            inventory.erase(inventory.begin());
            if(deleteItem){
                delete item;
            }
        }
    }

    bool removeItem(Item* item, bool deleteItem){
        forVector(inventory, i){
            Item* ie = inventory[i];
            if(ie == item){
                inventory.erase(inventory.begin()+(long)i);
                if(deleteItem){
                    delete item;
                }
                return true;
            }
        }
        return false;
    }

    virtual void addItems(vector<Item*> items){
        for(Item* i : items){
            addItem(i);
        }
    }

    virtual bool addItem(Item* newItem){
        if(newItem != nullptr){
            for(Item* i : inventory){
                if(i->equalsExceptQty(newItem)){
                    i->qty += newItem->qty;
                    delete newItem;
                    return true;
                }
            }
            inventory.push_back(newItem);
            return true;
        }
        return false;
    }
	
    vector<Item*> inventory;

};

namespace Ui{
    void drawInventory(Inventory* inv, int selected, int scrollOffset, string display, Weapon* activeWeapon = nullptr);
}

#endif /* defined(__Underneath__Inventory__) */

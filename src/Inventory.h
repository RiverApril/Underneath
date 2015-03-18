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

struct Inventory{

    Inventory(){

    }

    virtual ~Inventory(){
        
    }

    virtual void removeAllItems(bool deleteItem){
        while(inventory.size() > 0){
            Item* item = inventory[0];
            inventory.erase(inventory.begin());
            if(deleteItem){
                delete item;
            }
        }
    }

    virtual bool removeItem(Item* item, bool deleteItem){
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

    virtual bool containsItem(Item* item){
        forVector(inventory, i){
            Item* ie = inventory[i];
            if(ie == item){
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

#endif /* defined(__Underneath__Inventory__) */

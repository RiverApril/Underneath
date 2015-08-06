//
//  Inventory.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Inventory.hpp"
#include "ItemGenerator.hpp"

bool Inventory::addItem(Item* newItem, int qty) {
    if(qty != -1){
        newItem->qty = qty;
    }
    if (newItem != nullptr) {
        for (Item* i : inventory) {
            if (i->equalsExceptQty(newItem)) {
                i->qty += newItem->qty;
                delete newItem;
                return true;
            }
        }
        newItem->coinValue = ItemGenerator::calculateItemValue(newItem);
        inventory.push_back(newItem);
        return true;
    }
    return false;
}

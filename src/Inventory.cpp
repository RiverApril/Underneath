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
        newItem->coinValue = getCoinValue(newItem);
        inventory.push_back(newItem);
        return true;
    }
    return false;
}

int Inventory::getCoinValue(Item* item){
    return ItemGenerator::calculateItemValue(item);
}

int Inventory::getWallet(){
    int amount = 0;
    for (Item* i : inventory) {
        if(i->equalsExceptQty(ItemGenerator::iCoin)){
            amount += i->qty;
        }
    }
    return amount;
}

void Inventory::addToWallet(int amount){
    if(amount > 0){
        addItem(ItemGenerator::makeCoins(amount));
    }else if(amount < 0){
        amount = -amount;
        for(Item* i : inventory){
            if(i->equalsExceptQty(ItemGenerator::iCoin)){
                if(i->qty > amount){
                    i->qty -= amount;
                    amount = 0;
                    return;
                }else{
                    amount -= i->qty;
                    removeItem(i, true);
                }
            }
        }
    }
}

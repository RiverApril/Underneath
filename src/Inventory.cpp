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
        if(i->equalsExceptQty(ItemGenerator::coin->original())){
            amount += i->qty;
        }
    }
    return amount;
}

void Inventory::addToWallet(int amount){
    debugf("Add to Wallet: %d", amount);
    if(amount > 0){
        addItem(ItemGenerator::makeCoins(amount));
    }else if(amount < 0){
        amount = -amount;
        for(Item* i : inventory){
            if(i->equalsExceptQty(ItemGenerator::coin->original())){
                if(i->qty > amount){
                    debugf("BEFORE: i->qty=%d  amount=%d", i->qty, amount);
                    i->qty -= amount;
                    amount = 0;
                    debugf("AFTER: i->qty=%d  amount=%d", i->qty, amount);
                    return;
                }else{
                    amount -= i->qty;
                    removeItem(i, true);
                }
            }
        }
    }
}

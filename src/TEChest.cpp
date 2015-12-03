//
//  TEChest.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TEChest.hpp"
#include "Utility.hpp"

void TEChest::save(vector<unsigned char>* data) {
    TileEntity::save(data);

    Utility::saveInt(data, (int) inventory.size());

    forVector(inventory, i) {
        inventory[i]->save(data);
    }

}

void TEChest::load(vector<unsigned char>* data, int* position) {
    TileEntity::load(data, position);

    int size = Utility::loadInt(data, position);

    repeat(size, i) {
        inventory.push_back(Item::loadNew(data, position));
    }

}

TEChest* TEChest::cloneUnsafe(TEChest* oldE, TEChest* newE) {

    TileEntity::cloneUnsafe(oldE, newE);

    forVector(oldE->inventory, i) {
        newE->inventory.push_back(Item::clone(oldE->inventory[i]));
    }

    return newE;

}

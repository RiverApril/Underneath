//
//  ItemPotion.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemPotion.hpp"
#include "Utility.hpp"

void ItemPotion::save(vector<unsigned char>* data) {
    Item::save(data);

    Utility::saveInt(data, (int) effects.size());
    for (Effect e : effects) {
        e.save(data);
    }
}

void ItemPotion::load(vector<unsigned char>* data, int* position) {
    Item::load(data, position);

    int size = Utility::loadInt(data, position);
    effects.clear();

    repeat(size, i) {
        effects.push_back(Effect(data, position));
    }
}

ItemPotion* ItemPotion::cloneUnsafe(ItemPotion* oldE, ItemPotion* newE) {

    Item::cloneUnsafe(oldE, newE);

    newE->effects = oldE->effects;

    return newE;

}

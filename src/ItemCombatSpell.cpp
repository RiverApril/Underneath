//
//  ItemCombatSpell.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "ItemCombatSpell.hpp"
#include "Utility.hpp"

void ItemCombatSpell::save(vector<unsigned char>* data) {
    Ranged::save(data);

    Utility::saveInt(data, manaCost);
}

void ItemCombatSpell::load(unsigned char* data, int* position) {
    Ranged::load(data, position);

    manaCost = Utility::loadInt(data, position);
}

ItemCombatSpell* ItemCombatSpell::cloneUnsafe(ItemCombatSpell* oldE, ItemCombatSpell* newE) {

    Ranged::cloneUnsafe(oldE, newE);

    newE->manaCost = oldE->manaCost;


    return newE;

}

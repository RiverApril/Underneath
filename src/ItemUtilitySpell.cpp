//
//  UtiitySpell.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/17/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemUtilitySpell.hpp"
#include "Utility.hpp"

void ItemUtilitySpell::save(vector<unsigned char>* data) {
    Item::save(data);

    Utility::saveInt(data, spellEffect);
    Utility::saveInt(data, manaCost);
    Utility::saveBool(data, continuousUse);
}

void ItemUtilitySpell::load(unsigned char* data, int* position) {
    Item::load(data, position);

    spellEffect = Utility::loadInt(data, position);
    manaCost = Utility::loadInt(data, position);
    continuousUse = Utility::loadBool(data, position);
}

ItemUtilitySpell* ItemUtilitySpell::cloneUnsafe(ItemUtilitySpell* oldE, ItemUtilitySpell* newE) {

    Item::cloneUnsafe(oldE, newE);

    newE->spellEffect = oldE->spellEffect;
    newE->manaCost = oldE->manaCost;
    newE->continuousUse = oldE->continuousUse;

    return newE;

}

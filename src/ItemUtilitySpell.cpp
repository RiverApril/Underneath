//
//  UtiitySpell.cpp
//  Underneath
//
//  Created by Emily Atlee on 3/17/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "ItemUtilitySpell.hpp"
#include "Utility.hpp"

void ItemUtilitySpell::save(vector<unsigned char>* data) {
    Item::save(data);

    Utility::saveInt(data, spellEffect);
    Utility::saveDouble(data, manaCost);
    Utility::saveBool(data, continuousUse);
}

void ItemUtilitySpell::load(vector<unsigned char>* data, int* position) {
    Item::load(data, position);

    spellEffect = (SpellEffect)Utility::loadInt(data, position);
    manaCost = Utility::loadDouble(data, position);
    continuousUse = Utility::loadBool(data, position);
}

//
//  ItemAreaOfEffectWeapon.cpp
//  Underneath
//
//  Created by Emily Atlee on 3/14/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#include "ItemAreaOfEffectWeapon.hpp"
#include "Utility.hpp"

void ItemAreaOfEffectWeapon::save(vector<unsigned char>* data) {
    ItemCombatSpell::save(data);

    Utility::saveInt(data, radius);
}

void ItemAreaOfEffectWeapon::load(vector<unsigned char>* data, int* position) {
    ItemCombatSpell::load(data, position);

    radius = Utility::loadInt(data, position);
}


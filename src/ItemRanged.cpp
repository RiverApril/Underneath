//
//  ItemRanged.cpp
//  Underneath
//
//  Created by Emily Atlee on 1/22/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "ItemRanged.hpp"
#include "Utility.hpp"

void ItemRanged::save(vector<unsigned char>* data) {
    ItemWeapon::save(data);

    Utility::saveDouble(data, range);
    Utility::saveInt(data, (int)rangedType);
}

void ItemRanged::load(vector<unsigned char>* data, int* position) {
    ItemWeapon::load(data, position);

    range = Utility::loadDouble(data, position);
    rangedType = (RangedType)Utility::loadInt(data, position);
}

//
//  Ranged.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ranged.hpp"
#include "Utility.hpp"

void Ranged::save(vector<unsigned char>* data) {
    ItemWeapon::save(data);

    Utility::saveDouble(data, range);
}

void Ranged::load(unsigned char* data, int* position) {
    ItemWeapon::load(data, position);

    range = Utility::loadDouble(data, position);
}

Ranged* Ranged::cloneUnsafe(Ranged* oldE, Ranged* newE) {

    ItemWeapon::cloneUnsafe(oldE, newE);

    newE->range = oldE->range;


    return newE;

}

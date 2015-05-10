//
//  Weakness.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/16/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Weakness.hpp"

void Weakness::save(vector<unsigned char>* data) {
    Utility::saveInt(data, damageType);
    Utility::saveDouble(data, multiplier);
}

void Weakness::load(unsigned char* data, int* position) {
    damageType = Utility::loadInt(data, position);
    multiplier = Utility::loadDouble(data, position);
}

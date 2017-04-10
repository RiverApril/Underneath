//
//  TEStair.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TEStair.hpp"
#include "Utility.hpp"

void TEStair::save(vector<unsigned char>* data) {
    TileEntity::save(data);

    Utility::saveBool(data, up);
    Utility::saveString(data, levelName);
}

void TEStair::load(vector<unsigned char>* data, int* position) {
    TileEntity::load(data, position);

    up = Utility::loadBool(data, position);
    levelName = Utility::loadString(data, position);
}

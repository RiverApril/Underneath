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

void TEStair::load(unsigned char* data, int* position) {
    TileEntity::load(data, position);

    up = Utility::loadBool(data, position);
    levelName = Utility::loadString(data, position);
}

TEStair* TEStair::cloneUnsafe(TEStair* oldE, TEStair* newE) {

    TileEntity::cloneUnsafe(oldE, newE);

    newE->up = oldE->up;
    newE->levelName = oldE->levelName;

    return newE;

}

//
//  TEStair.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TEStair.h"
#include "Utility.h"

void TEStair::save(vector<unsigned char>* data){
    TileEntity::save(data);

    FileUtility::saveBool(data, up);
    FileUtility::saveString(data, levelName);
}

void TEStair::load(unsigned char* data, int* position){
    TileEntity::load(data, position);

    up = FileUtility::loadBool(data, position);
    levelName = FileUtility::loadString(data, position);
}

TEStair* TEStair::cloneUnsafe(TEStair* oldE, TEStair* newE){

    TileEntity::cloneUnsafe(oldE, newE);

    newE->up = oldE->up;
    newE->levelName = oldE->levelName;

    return newE;
    
}
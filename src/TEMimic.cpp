//
//  TEMimic.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/28/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TEMimic.h"
#include "Utility.h"

void TEMimic::save(vector<unsigned char>* data) {
    TileEntity::save(data);
}

void TEMimic::load(unsigned char* data, int* position) {
    TileEntity::load(data, position);
}

TEMimic* TEMimic::cloneUnsafe(TEMimic* oldE, TEMimic* newE) {

    TileEntity::cloneUnsafe(oldE, newE);

    return newE;

}

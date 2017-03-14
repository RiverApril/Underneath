//
//  TEAlter.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/11/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "TEAlter.hpp"
#include "Utility.hpp"

void TEAlter::save(vector<unsigned char>* data) {
    TileEntity::save(data);
    Utility::saveInt(data, offerIndex);
}

void TEAlter::load(vector<unsigned char>* data, int* position) {
    TileEntity::load(data, position);
    offerIndex = Utility::loadInt(data, position);
}

TEAlter* TEAlter::cloneUnsafe(TEAlter* oldE, TEAlter* newE) {
    
    TileEntity::cloneUnsafe(oldE, newE);

    newE->offerIndex = oldE->offerIndex;
    
    return newE;
    
}

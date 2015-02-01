//
//  Ranged.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ranged.h"
#include "Utility.h"

void Ranged::save(vector<unsigned char>* data){
    Weapon::save(data);

    FileUtility::saveDouble(data, range);
}

void Ranged::load(unsigned char* data, int* position){
    Weapon::load(data, position);

    range = FileUtility::loadDouble(data, position);
}

Ranged* Ranged::cloneUnsafe(Ranged* oldE, Ranged* newE){

    Weapon::cloneUnsafe(oldE, newE);

    newE->range = oldE->range;


    return newE;
    
}

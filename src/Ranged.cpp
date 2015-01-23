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

    FileUtility::saveInt(data, range);
}

void Ranged::load(unsigned char* data, int* position){
    Weapon::load(data, position);

    baseDamage = FileUtility::loadInt(data, position);
    range = FileUtility::loadInt(data, position);
}

int Ranged::getItemTypeId(){
    return ITEM_TYPE_RANGED;
}

Ranged* Ranged::clone(Ranged* oldE, Ranged* newE){

    if(newE == nullptr){
        newE = new Ranged();
    }

    Weapon::clone(oldE, newE);

    newE->range = oldE->range;


    return newE;
    
}

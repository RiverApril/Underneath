//
//  Spell.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Spell.h"
#include "Utility.h"

void Spell::save(vector<unsigned char>* data){
    Ranged::save(data);

    FileUtility::saveInt(data, manaCost);
}

void Spell::load(unsigned char* data, int* position){
    Ranged::load(data, position);

    manaCost = FileUtility::loadInt(data, position);
}

Spell* Spell::cloneUnsafe(Spell* oldE, Spell* newE){

    Ranged::cloneUnsafe(oldE, newE);

    newE->manaCost = oldE->manaCost;


    return newE;
    
}

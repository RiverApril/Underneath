//
//  UtiitySpell.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/17/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "UtilitySpell.h"
#include "Utility.h"

void UtilitySpell::save(vector<unsigned char>* data){
    Item::save(data);

    Utility::saveInt(data, spellEffect);
    Utility::saveInt(data, manaCost);
}

void UtilitySpell::load(unsigned char* data, int* position){
    Item::load(data, position);

    spellEffect = Utility::loadInt(data, position);
    manaCost = Utility::loadInt(data, position);
}

UtilitySpell* UtilitySpell::cloneUnsafe(UtilitySpell* oldE, UtilitySpell* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->spellEffect = oldE->spellEffect;

    return newE;

}

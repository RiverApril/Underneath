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
    Item::save(data);

    FileUtility::saveInt(data, baseDamage);
    FileUtility::saveInt(data, manaCost);
    FileUtility::saveDouble(data, castDelay);
}

void Spell::load(unsigned char* data, int* position){
    Item::load(data, position);

    baseDamage = FileUtility::loadInt(data, position);
    manaCost = FileUtility::loadInt(data, position);
    castDelay = FileUtility::loadDouble(data, position);
}

int Spell::getItemTypeId(){
    return ITEM_TYPE_SPELL;
}

Spell* Spell::clone(Spell* oldE, Spell* newE){

    if(newE == nullptr){
        newE = new Spell();
    }

    Spell::clone(oldE, newE);

    newE->baseDamage = oldE->baseDamage;
    newE->manaCost = oldE->manaCost;
    newE->castDelay = oldE->castDelay;


    return newE;
    
}

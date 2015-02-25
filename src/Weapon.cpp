//
//  Weapon.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Weapon.h"
#include "Utility.h"

void Weapon::save(vector<unsigned char>* data){
    MaterialItem::save(data);
    
    FileUtility::saveDouble(data, baseDamage);
    FileUtility::saveInt(data, damageType);
    FileUtility::saveDouble(data, useDelay);

    FileUtility::saveInt(data, enchantments.size());
    for(size_t i=0;i<enchantments.size();i++){
        FileUtility::saveInt(data, enchantments[i].eId);
        FileUtility::saveInt(data, enchantments[i].chance);
        FileUtility::saveInt(data, enchantments[i].power);
    }
}

void Weapon::load(unsigned char* data, int* position){
    MaterialItem::load(data, position);

    baseDamage = FileUtility::loadDouble(data, position);
    damageType = FileUtility::loadInt(data, position);
    useDelay = FileUtility::loadDouble(data, position);

    int size = FileUtility::loadInt(data, position);
    for(int i=0;i<size;i++){
        int eId = FileUtility::loadInt(data, position);
        int chance = FileUtility::loadInt(data, position);
        int power = FileUtility::loadInt(data, position);
        enchantments.push_back(Enchantment(eId, chance, power));
    }
}

Weapon* Weapon::cloneUnsafe(Weapon* oldE, Weapon* newE){

    MaterialItem::cloneUnsafe(oldE, newE);

    newE->baseDamage = oldE->baseDamage;
    newE->enchantments = oldE->enchantments;
    newE->damageType = oldE->damageType;
    newE->useDelay = oldE->useDelay;

    
    return newE;
    
}

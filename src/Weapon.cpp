//
//  Weapon.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Weapon.h"
#include "Utility.h"

void Weapon::save(string* data){
    Item::save(data);
    
    FileUtility::saveInt(data, baseDamage);

    FileUtility::saveInt(data, (int)enchantments.size());
    for(int i=0;i<enchantments.size();i++){
        FileUtility::saveInt(data, enchantments[i].eId);
        FileUtility::saveInt(data, enchantments[i].chance);
        FileUtility::saveInt(data, enchantments[i].power);
    }
}

void Weapon::load(char* data, int* position){
    Item::load(data, position);

    baseDamage = FileUtility::loadInt(data, position);

    int size = FileUtility::loadInt(data, position);
    for(int i=0;i<size;i++){
        int eId = FileUtility::loadInt(data, position);
        int chance = FileUtility::loadInt(data, position);
        int power = FileUtility::loadInt(data, position);
        enchantments.push_back(Enchantment(eId, chance, power));
    }
}

int Weapon::getItemTypeId(){
    return ITEM_TYPE_WEAPON;
}

Weapon* Weapon::clone(Weapon* oldE, Weapon* newE){

    if(newE == nullptr){
        newE = new Weapon();
    }

    Weapon::clone(oldE, newE);

    newE->baseDamage = oldE->baseDamage;
    newE->enchantments = oldE->enchantments;

    
    return newE;
    
}

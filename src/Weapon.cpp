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
    
    Utility::saveInt(data, baseDamage);

    Utility::saveInt(data, (int)enchantments.size());
    for(int i=0;i<enchantments.size();i++){
        Utility::saveInt(data, enchantments[i].eId);
        Utility::saveInt(data, enchantments[i].chance);
        Utility::saveInt(data, enchantments[i].power);
    }
}

void Weapon::load(char* data, int* position){
    Item::load(data, position);

    baseDamage = Utility::loadInt(data, position);

    int size = Utility::loadInt(data, position);
    for(int i=0;i<size;i++){
        int eId = Utility::loadInt(data, position);
        int chance = Utility::loadInt(data, position);
        int power = Utility::loadInt(data, position);
        enchantments.push_back(Enchantment(eId, chance, power));
    }
}

int Weapon::getItemTypeId(){
    return ITEM_TYPE_WEAPON;
}

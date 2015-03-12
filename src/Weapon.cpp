//
//  Weapon.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Weapon.h"
#include "Utility.h"

Weapon::Weapon(double baseDamage, string name, double weight, double useDelay) : Item(name, weight){
    debugf("%s: %.2f", name.c_str(), baseDamage);
    this->baseDamage = baseDamage;
    this->useDelay = useDelay;

    this->weaponType = wepMelee;
    this->damageType = damSharp;
}

void Weapon::save(vector<unsigned char>* data){
    Item::save(data);
    
    Utility::saveDouble(data, baseDamage);
    Utility::saveInt(data, damageType);
    Utility::saveDouble(data, useDelay);

    Utility::saveInt(data, (int)enchantments.size());
    for(size_t i=0;i<enchantments.size();i++){
        enchantments[i].save(data);
        //Utility::saveInt(data, enchantments[i].eId);
        //Utility::saveInt(data, enchantments[i].chance);
        //Utility::saveInt(data, enchantments[i].power);
    }
}

void Weapon::load(unsigned char* data, int* position){
    Item::load(data, position);

    baseDamage = Utility::loadDouble(data, position);
    damageType = Utility::loadInt(data, position);
    useDelay = Utility::loadDouble(data, position);

    int size = Utility::loadInt(data, position);
    for(int i=0;i<size;i++){
        //int eId = Utility::loadInt(data, position);
        //int chance = Utility::loadInt(data, position);
        //int power = Utility::loadInt(data, position);
        enchantments.push_back(Enchantment(data, position));
    }
}

Weapon* Weapon::cloneUnsafe(Weapon* oldE, Weapon* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->baseDamage = oldE->baseDamage;
    newE->enchantments = oldE->enchantments;
    newE->damageType = oldE->damageType;
    newE->useDelay = oldE->useDelay;

    
    return newE;
    
}

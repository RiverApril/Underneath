//
//  Weapon.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Weapon.hpp"
#include "Utility.hpp"

Weapon::Weapon(double baseDamage, string name, double weight, double useDelay, bool twoHanded) : Equipable(name, weight) {
    debugf("%s: %.2f", name.c_str(), baseDamage);
    this->baseDamage = baseDamage;
    this->useDelay = useDelay;

    this->weaponType = wepMelee;
    this->damageType = damSharp;

    this->twoHanded = twoHanded;
}

void Weapon::save(vector<unsigned char>* data) {
    Equipable::save(data);

    Utility::saveDouble(data, baseDamage);
    Utility::saveInt(data, damageType);
    Utility::saveDouble(data, useDelay);
    Utility::saveBool(data, twoHanded);

    Utility::saveInt(data, (int) enchantments.size());
    for (size_t i = 0; i < enchantments.size(); i++) {
        enchantments[i].save(data);
        //Utility::saveInt(data, enchantments[i].eId);
        //Utility::saveInt(data, enchantments[i].chance);
        //Utility::saveInt(data, enchantments[i].power);
    }
}

void Weapon::load(unsigned char* data, int* position) {
    Equipable::load(data, position);

    baseDamage = Utility::loadDouble(data, position);
    damageType = Utility::loadInt(data, position);
    useDelay = Utility::loadDouble(data, position);
    twoHanded = Utility::loadBool(data, position);

    int size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        //int eId = Utility::loadInt(data, position);
        //int chance = Utility::loadInt(data, position);
        //int power = Utility::loadInt(data, position);
        enchantments.push_back(Enchantment(data, position));
    }
}

Weapon* Weapon::cloneUnsafe(Weapon* oldE, Weapon* newE) {

    Equipable::cloneUnsafe(oldE, newE);

    newE->baseDamage = oldE->baseDamage;
    newE->enchantments = oldE->enchantments;
    newE->damageType = oldE->damageType;
    newE->useDelay = oldE->useDelay;
    newE->twoHanded = oldE->twoHanded;


    return newE;

}

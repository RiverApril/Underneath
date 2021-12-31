//
//  ItemWeapon.cpp
//  Underneath
//
//  Created by Emily Atlee on 11/11/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#include "ItemWeapon.hpp"
#include "Utility.hpp"

ItemWeapon::ItemWeapon(double baseDamage, string name, double useDelay) : ItemEquipable(name) {
    debugf("%s: %.2f", name.c_str(), baseDamage);
    this->baseDamage = baseDamage;
    this->useDelay = useDelay;

    this->weaponType = wepMelee;
    this->damageType = damSharp;
}

void ItemWeapon::save(vector<unsigned char>* data) {
    ItemEquipable::save(data);

    Utility::saveDouble(data, baseDamage);
    Utility::saveInt(data, damageType);
    Utility::saveDouble(data, useDelay);

    Utility::saveInt(data, (int) enchantments.size());
    for (size_t i = 0; i < enchantments.size(); i++) {
        enchantments[i].save(data);
        //Utility::saveInt(data, enchantments[i].eId);
        //Utility::saveInt(data, enchantments[i].chance);
        //Utility::saveInt(data, enchantments[i].power);
    }
}

void ItemWeapon::load(vector<unsigned char>* data, int* position) {
    ItemEquipable::load(data, position);

    baseDamage = Utility::loadDouble(data, position);
    damageType = (DamageType)Utility::loadInt(data, position);
    useDelay = Utility::loadDouble(data, position);

    int size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        //int eId = Utility::loadInt(data, position);
        //int chance = Utility::loadInt(data, position);
        //int power = Utility::loadInt(data, position);
        enchantments.push_back(Enchantment(data, position));
    }
}

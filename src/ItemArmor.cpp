//
//  ItemArmor.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemArmor.hpp"
#include "Utility.hpp"

ItemArmor::ItemArmor(vector<EquipSlot> viableSlots, vector<Defense> defenses, string name) : ItemEquipable(name) {
    this->viableSlots = viableSlots;
    this->defenses = defenses;
}

ItemArmor::ItemArmor(EquipSlot viableSlot, vector<Defense> defenses, string name) : ItemArmor(vector<EquipSlot>{viableSlot}, defenses, name) {
}

void ItemArmor::save(vector<unsigned char>* data) {
    ItemEquipable::save(data);

    Utility::saveInt(data, (int) defenses.size());
    for (Defense def : defenses) {
        def.save(data);
    }

    Utility::saveInt(data, (int) viableSlots.size());
    for (EquipSlot slot : viableSlots) {
        Utility::saveInt(data, slot);
    }
}

void ItemArmor::load(vector<unsigned char>* data, int* position) {
    ItemEquipable::load(data, position);

    int size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        defenses.push_back(Defense(data, position));
    }

    size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        viableSlots.push_back((EquipSlot)Utility::loadInt(data, position));
    }
}

ItemArmor* ItemArmor::cloneUnsafe(ItemArmor* oldE, ItemArmor* newE) {

    ItemEquipable::cloneUnsafe(oldE, newE);

    newE->defenses = oldE->defenses;
    newE->enchantments = oldE->enchantments;
    newE->viableSlots = oldE->viableSlots;
    
    
    return newE;
    
}

bool operator==(const Defense& a, const Defense& b){
    return a.amount == b.amount && a.damageType == b.damageType;
}

bool operator!=(const Defense& a, const Defense& b){
    return !(a==b);
}

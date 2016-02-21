//
//  ItemEquipable.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemEquipable.hpp"
#include "Utility.hpp"

ItemEquipable::ItemEquipable(string name) : Item(name) {

}

void ItemEquipable::save(vector<unsigned char>* data) {
    Item::save(data);

    minimumAbilities.save(data);

    Utility::saveInt(data, (int) enchantments.size());
    for (Enchantment ench : enchantments) {
        ench.save(data);
    }
}

void ItemEquipable::load(vector<unsigned char>* data, int* position) {
    Item::load(data, position);

    minimumAbilities.load(data, position);

    int size = Utility::loadInt(data, position);
    for (int i = 0; i < size; i++) {
        enchantments.push_back(Enchantment(data, position));
    }
}

ItemEquipable* ItemEquipable::cloneUnsafe(ItemEquipable* oldE, ItemEquipable* newE) {

    Item::cloneUnsafe(oldE, newE);

    newE->minimumAbilities = oldE->minimumAbilities;
    //newE->durability = oldE->durability;


    return newE;

}

bool ItemEquipable::equalsExceptQty(Item* other) {
    ItemEquipable* otherW = dynamic_cast<ItemEquipable*> (other);
    return (otherW)
    		&&Item::equalsExceptQty(other)
    		&& (enchantments == otherW->enchantments)
            &&(otherW->minimumAbilities == minimumAbilities);
}

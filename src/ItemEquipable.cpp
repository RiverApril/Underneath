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
}

void ItemEquipable::load(unsigned char* data, int* position) {
    Item::load(data, position);

    minimumAbilities.load(data, position);
}

ItemEquipable* ItemEquipable::cloneUnsafe(ItemEquipable* oldE, ItemEquipable* newE) {

    Item::cloneUnsafe(oldE, newE);

    newE->minimumAbilities = oldE->minimumAbilities;

    return newE;

}

bool ItemEquipable::equalsExceptQty(Item* other) {
    ItemEquipable* otherW = dynamic_cast<ItemEquipable*> (other);
    return Item::equalsExceptQty(other)
            &&(otherW)
            &&(otherW->minimumAbilities == minimumAbilities);
}

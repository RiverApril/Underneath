//
//  ItemExplosive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemExplosive.hpp"
#include "Utility.hpp"
#include "Math.hpp"

ItemExplosive::ItemExplosive(string name, ExplosiveType explosiveType, double time, double power, double radius, bool destroysTiles) : Item(name){
    this->explosiveType = explosiveType;
    this->time = time;
    this->power = power;
    this->radius = radius;
    this->destroysTiles = destroysTiles;
}

void ItemExplosive::save(vector<unsigned char>* data){
    Item::save(data);
    Utility::saveInt(data, explosiveType);
    Utility::saveDouble(data, time);
    Utility::saveDouble(data, power);
    Utility::saveDouble(data, radius);
    Utility::saveBool(data, destroysTiles);
    Utility::saveInt(data, iconIndex);
}

void ItemExplosive::load(vector<unsigned char>* data, int* position){
    Item::load(data, position);
    explosiveType = (ExplosiveType)Utility::loadInt(data, position);
    time = Utility::loadDouble(data, position);
    power = Utility::loadDouble(data, position);
    radius = Utility::loadDouble(data, position);
    destroysTiles = Utility::loadBool(data, position);
    iconIndex = Utility::loadInt(data, position);
}

ItemExplosive* ItemExplosive::cloneUnsafe(ItemExplosive* oldE, ItemExplosive* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->explosiveType = oldE->explosiveType;
    newE->time = oldE->time;
    newE->power = oldE->power;
    newE->radius = oldE->radius;
    newE->destroysTiles = oldE->destroysTiles;
    newE->iconIndex = oldE->iconIndex;

    return newE;

}

UseType ItemExplosive::getUseType(){
    switch (explosiveType) {
        case dudBomb:
            return useInstant;

        case timeActivatedBomb:
            return useInstant;

        case pressureBomb:
            return useInstant;

        case throwableBomb:
            return useInWorld;

        default:
            return useInstant;
    }
}

bool ItemExplosive::equalsExceptQty(Item* other){
    ItemExplosive* otherW = dynamic_cast<ItemExplosive*>(other);
    return (otherW)
    	&& Item::equalsExceptQty(other)
        && (otherW->explosiveType == explosiveType)
        && aproxEqual(otherW->time, time)
        && aproxEqual(otherW->power, power)
        && aproxEqual(otherW->radius, radius)
        && otherW->destroysTiles == destroysTiles;
}

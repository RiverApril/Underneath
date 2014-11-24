//
//  Item.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Item.h"
#include "Utility.h"
#include "Weapon.h"

void Item::save(string* data){
    FileUtility::saveInt(data, getItemTypeId());
    FileUtility::saveString(data, name);
}

void Item::load(char* data, int* position){
    name = FileUtility::loadString(data, position);
}

int Item::getItemTypeId(){
    return ITEM_TYPE_ITEM;
}

Item* Item::clone(Item* oldE, Item* newE){

    if(newE == nullptr){
        newE = new Item();
    }

    newE->name = oldE->name;

    return newE;
}

Item* Item::loadNew(char* data, int* position){
    Item* e;

    int type = FileUtility::loadInt(data, position);

    switch (type) {
        case ITEM_TYPE_ITEM:
            e = new Item();
            break;
        case ITEM_TYPE_WEAPON:
            e = new Weapon();
            break;
            
        default:
            throw FileUtility::ExceptionLoad("Item type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}

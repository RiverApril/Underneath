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
    Utility::saveInt(data, getItemTypeId());
    Utility::saveString(data, name);
}

void Item::load(char* data, int* position){
    name = Utility::loadString(data, position);
}

int Item::getItemTypeId(){
    return ITEM_TYPE_ITEM;
}

shared_ptr<Item> Item::loadNew(char* data, int* position){
    shared_ptr<Item> e;

    int type = Utility::loadInt(data, position);

    if(type == ITEM_TYPE_ITEM){
        e = shared_ptr<Item>(new Item());
    }else if(type == ITEM_TYPE_WEAPON){
        e = shared_ptr<Weapon>(new Weapon());
    }
    e->load(data, position);

    return e;
}

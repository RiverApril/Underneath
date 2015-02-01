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
#include "Spell.h"

void Item::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, getItemTypeId());
    FileUtility::saveDouble(data, weight);
    FileUtility::saveString(data, name);
    FileUtility::saveInt(data, qty);
}

void Item::load(unsigned char* data, int* position){
    weight = FileUtility::loadDouble(data, position);
    name = FileUtility::loadString(data, position);
    qty = FileUtility::loadInt(data, position);
}

Item* Item::cloneUnsafe(Item* oldE, Item* newE){

    newE->name = oldE->name;
    newE->weight = oldE->weight;

    return newE;
}

template<class Super, class Sub>
Sub* Item::makeNewAndClone(Super* oldT){
    Sub* newT = new Sub();
	return Sub::cloneUnsafe(dynamic_cast<Sub*>(oldT), newT);
}

Item* Item::clone(Item* oldI){

    int type = oldI->getItemTypeId();

    switch (type) {
        case ITEM_TYPE_ITEM:
            return makeNewAndClone<Item, Item>(oldI);

        case ITEM_TYPE_WEAPON:
            return makeNewAndClone<Item, Weapon>(oldI);

        case ITEM_TYPE_RANGED:
            return makeNewAndClone<Item, Ranged>(oldI);

        case ITEM_TYPE_SPELL:
            return makeNewAndClone<Item, Spell>(oldI);

        default:
            throw FileUtility::FileExceptionLoad("Item type unknown: "+to_string(type));
            return nullptr;
            break;
    }



}

Item* Item::loadNew(unsigned char* data, int* position){
    Item* e;

    int type = FileUtility::loadInt(data, position);

    switch (type) {
        case ITEM_TYPE_ITEM:
            e = new Item();
            break;
        case ITEM_TYPE_WEAPON:
            e = new Weapon();
            break;
        case ITEM_TYPE_RANGED:
            e = new Ranged();
            break;
        case ITEM_TYPE_SPELL:
            e = new Spell();
            break;
            
        default:
            throw FileUtility::FileExceptionLoad("Item type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}

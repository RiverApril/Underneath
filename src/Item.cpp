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
#include "Potion.h"

void Item::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, getItemTypeId());
    FileUtility::saveDouble(data, weight);
    FileUtility::saveString(data, name);
    FileUtility::saveInt(data, qty);
    FileUtility::saveInt(data, artIndex);
}

void Item::load(unsigned char* data, int* position){
    weight = FileUtility::loadDouble(data, position);
    name = FileUtility::loadString(data, position);
    qty = FileUtility::loadInt(data, position);
    artIndex = FileUtility::loadInt(data, position);
}

Item* Item::cloneUnsafe(Item* oldE, Item* newE){

    newE->name = oldE->name;
    newE->weight = oldE->weight;
    newE->artIndex = oldE->artIndex;

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

        case ITEM_TYPE_MATERIAL_ITEM:
            return makeNewAndClone<Item, MaterialItem>(oldI);

        case ITEM_TYPE_WEAPON:
            return makeNewAndClone<Item, Weapon>(oldI);

        case ITEM_TYPE_RANGED:
            return makeNewAndClone<Item, Ranged>(oldI);

        case ITEM_TYPE_SPELL:
			return makeNewAndClone<Item, Spell>(oldI);

		case ITEM_TYPE_POTION:
			return makeNewAndClone<Item, Potion>(oldI);

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
        case ITEM_TYPE_MATERIAL_ITEM:
            e = new MaterialItem();
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
		case ITEM_TYPE_POTION:
			e = new Potion();
			break;
            
        default:
            throw FileUtility::FileExceptionLoad("Item type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}

//
//  Item.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Item.h"
#include "Utility.h"
#include "Equipable.h"
#include "Weapon.h"
#include "CombatSpell.h"
#include "Potion.h"
#include "UtilitySpell.h"
#include "ItemSpecial.h"
#include "ItemTimeActivated.h"
#include "Armor.h"

void Item::save(vector<unsigned char>* data) {
    Utility::saveInt(data, getItemTypeId());
    Utility::saveDouble(data, weight);
    Utility::saveString(data, name);
    Utility::saveInt(data, qty);
    Utility::saveInt(data, artIndex);
}

void Item::load(unsigned char* data, int* position) {
    weight = Utility::loadDouble(data, position);
    name = Utility::loadString(data, position);
    qty = Utility::loadInt(data, position);
    artIndex = Utility::loadInt(data, position);
}

Item* Item::cloneUnsafe(Item* oldE, Item* newE) {

    newE->qty = oldE->qty;
    newE->name = oldE->name;
    newE->weight = oldE->weight;
    newE->artIndex = oldE->artIndex;

    return newE;
}

template<class Super, class Sub>
Sub* Item::makeNewAndClone(Super* oldT) {
    Sub* newT = new Sub();
    return Sub::cloneUnsafe(dynamic_cast<Sub*> (oldT), newT);
}

Item* Item::clone(Item* oldI) {

    int type = oldI->getItemTypeId();

    switch (type) {
        case ITEM_TYPE_ITEM:
            return makeNewAndClone<Item, Item>(oldI);

        case ITEM_TYPE_EQUIPABLE:
            return makeNewAndClone<Item, Equipable>(oldI);

        case ITEM_TYPE_WEAPON:
            return makeNewAndClone<Item, Weapon>(oldI);

        case ITEM_TYPE_RANGED:
            return makeNewAndClone<Item, Ranged>(oldI);

        case ITEM_TYPE_COMBAT_SPELL:
            return makeNewAndClone<Item, CombatSpell>(oldI);

        case ITEM_TYPE_POTION:
            return makeNewAndClone<Item, Potion>(oldI);

		case ITEM_TYPE_UTILITY_SPELL:
            return makeNewAndClone<Item, UtilitySpell>(oldI);

        case ITEM_TYPE_ITEMSPECIAL:
            return makeNewAndClone<Item, ItemSpecial>(oldI);

        case ITEM_TYPE_TIME_ACTIVATED:
            return makeNewAndClone<Item, ItemTimeActivated>(oldI);

        case ITEM_TYPE_ARMOR:
            return makeNewAndClone<Item, Armor>(oldI);

        default:
            throw Utility::FileExceptionLoad("Item type unknown: " + to_string(type));
            return nullptr;
            break;
    }



}

Item* Item::loadNew(unsigned char* data, int* position) {
    Item* e;

    int type = Utility::loadInt(data, position);

    switch (type) {
        case ITEM_TYPE_ITEM:
            e = new Item();
            break;

        case ITEM_TYPE_EQUIPABLE:
            e = new Equipable();
            break;

        case ITEM_TYPE_WEAPON:
            e = new Weapon();
            break;

        case ITEM_TYPE_RANGED:
            e = new Ranged();
            break;

        case ITEM_TYPE_COMBAT_SPELL:
            e = new CombatSpell();
            break;
            
        case ITEM_TYPE_POTION:
            e = new Potion();
            break;

        case ITEM_TYPE_UTILITY_SPELL:
            e = new UtilitySpell();
            break;

        case ITEM_TYPE_ITEMSPECIAL:
            e = new ItemSpecial();
            break;

        case ITEM_TYPE_TIME_ACTIVATED:
            e = new ItemTimeActivated();
            break;

        case ITEM_TYPE_ARMOR:
            e = new Armor();
            break;

        default:
            throw Utility::FileExceptionLoad("Item type unknown: " + to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}

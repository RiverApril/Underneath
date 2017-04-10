//
//  Item.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Item.hpp"
#include "Utility.hpp"
#include "ItemEquipable.hpp"
#include "ItemWeapon.hpp"
#include "ItemCombatSpell.hpp"
#include "ItemPotion.hpp"
#include "ItemUtilitySpell.hpp"
#include "ItemSpecial.hpp"
#include "ItemExplosive.hpp"
#include "ItemArmor.hpp"
#include "ItemAreaOfEffectWeapon.hpp"

void Item::save(vector<unsigned char>* data) {
    Utility::saveInt(data, getItemTypeId());
    Utility::saveString(data, name);
    Utility::saveString(data, pluralName);
    Utility::saveInt(data, qty);
    Utility::saveInt(data, artIndex);
    Utility::saveInt(data, coinValue);
}

void Item::load(vector<unsigned char>* data, int* position) {
    //loads id before
    name = Utility::loadString(data, position);
    pluralName = Utility::loadString(data, position);
    qty = Utility::loadInt(data, position);
    artIndex = Utility::loadInt(data, position);
    coinValue = Utility::loadInt(data, position);
}

Item* Item::clone(Item* oldI, int qty) {

    if(!oldI){
        return nullptr;
    }
    
    vector<unsigned char>* data = new vector<unsigned char>();
    oldI->save(data);
    int* position = new int(0);
    Item* i = loadNew(data, position);
    
    delete data;
    delete position;
    
    if(i && qty != -1){
        i->qty = qty;
    }
    
    return i;
}

Item* Item::loadNew(vector<unsigned char>* data, int* position) {
    Item* e;

    int type = Utility::loadInt(data, position);

    switch (type) {
        case itemTypeItem:
            e = new Item();
            break;

        case itemTypeEquipable:
            e = new ItemEquipable();
            break;

        case itemTypeWeapon:
            e = new ItemWeapon();
            break;

        case itemTypeRanged:
            e = new ItemRanged();
            break;

        case itemTypeCombatSpell:
            e = new ItemCombatSpell();
            break;

        case itemTypePotion:
            e = new ItemPotion();
            break;

        case itemTypeUtilitySpell:
            e = new ItemUtilitySpell();
            break;

        case itemTypeSpecial:
            e = new ItemSpecial();
            break;

        case itemTypeExplosive:
            e = new ItemExplosive();
            break;

        case itemTypeArmor:
            e = new ItemArmor();
            break;

        case itemTypeAofWeapon:
            e = new ItemAreaOfEffectWeapon();
            break;

        default:
            throw Utility::FileExceptionLoad("Item type unknown: " + to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}

bool Item::operator< (Item& b){
    if(this->getItemTypeId() == b.getItemTypeId()){
        switch (this->getItemTypeId()) {
            case itemTypeItem:
                return this->qty < b.qty;

            case itemTypeEquipable:
                return *dynamic_cast<ItemEquipable*>(this) < dynamic_cast<ItemEquipable&>(b);

            case itemTypeWeapon:
                return *dynamic_cast<ItemWeapon*>(this) < dynamic_cast<ItemWeapon&>(b);

            case itemTypeRanged:
                return *dynamic_cast<ItemRanged*>(this) < dynamic_cast<ItemRanged&>(b);

            case itemTypeCombatSpell:
                return *dynamic_cast<ItemCombatSpell*>(this) < dynamic_cast<ItemCombatSpell&>(b);

            case itemTypePotion:
                return *dynamic_cast<ItemPotion*>(this) < dynamic_cast<ItemPotion&>(b);

            case itemTypeUtilitySpell:
                return *dynamic_cast<ItemUtilitySpell*>(this) < dynamic_cast<ItemUtilitySpell&>(b);

            case itemTypeSpecial:
                return *dynamic_cast<ItemSpecial*>(this) < dynamic_cast<ItemSpecial&>(b);

            case itemTypeExplosive:
                return *dynamic_cast<ItemExplosive*>(this) < dynamic_cast<ItemExplosive&>(b);

            case itemTypeArmor:
                return *dynamic_cast<ItemArmor*>(this) < dynamic_cast<ItemArmor&>(b);

            case itemTypeAofWeapon:
                return *dynamic_cast<ItemAreaOfEffectWeapon*>(this) < dynamic_cast<ItemAreaOfEffectWeapon&>(b);
        }
    }
    return this->getItemTypeId() < b.getItemTypeId();
}

bool comparePrice(Item* a, Item* b){
    return a->coinValue < b->coinValue;
}

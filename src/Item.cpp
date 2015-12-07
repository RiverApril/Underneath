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
#include "ItemTimeActivated.hpp"
#include "ItemArmor.hpp"

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

Item* Item::cloneUnsafe(Item* oldE, Item* newE) {

    newE->name = oldE->name;
    newE->pluralName = oldE->pluralName;
    newE->qty = oldE->qty;
    newE->artIndex = oldE->artIndex;
    newE->coinValue = oldE->coinValue;

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
            return makeNewAndClone<Item, ItemEquipable>(oldI);

        case ITEM_TYPE_WEAPON:
            return makeNewAndClone<Item, ItemWeapon>(oldI);

        case ITEM_TYPE_RANGED:
            return makeNewAndClone<Item, ItemRanged>(oldI);

        case ITEM_TYPE_COMBAT_SPELL:
            return makeNewAndClone<Item, ItemCombatSpell>(oldI);

        case ITEM_TYPE_POTION:
            return makeNewAndClone<Item, ItemPotion>(oldI);

		case ITEM_TYPE_UTILITY_SPELL:
            return makeNewAndClone<Item, ItemUtilitySpell>(oldI);

        case ITEM_TYPE_ITEMSPECIAL:
            return makeNewAndClone<Item, ItemSpecial>(oldI);

        case ITEM_TYPE_TIME_ACTIVATED:
            return makeNewAndClone<Item, ItemTimeActivated>(oldI);

        case ITEM_TYPE_ARMOR:
            return makeNewAndClone<Item, ItemArmor>(oldI);

        default:
            throw Utility::FileExceptionLoad("Item type unknown: " + to_string(type));
            return nullptr;
            break;
    }



}

Item* Item::loadNew(vector<unsigned char>* data, int* position) {
    Item* e;

    int type = Utility::loadInt(data, position);

    switch (type) {
        case ITEM_TYPE_ITEM:
            e = new Item();
            break;

        case ITEM_TYPE_EQUIPABLE:
            e = new ItemEquipable();
            break;

        case ITEM_TYPE_WEAPON:
            e = new ItemWeapon();
            break;

        case ITEM_TYPE_RANGED:
            e = new ItemRanged();
            break;

        case ITEM_TYPE_COMBAT_SPELL:
            e = new ItemCombatSpell();
            break;

        case ITEM_TYPE_POTION:
            e = new ItemPotion();
            break;

        case ITEM_TYPE_UTILITY_SPELL:
            e = new ItemUtilitySpell();
            break;

        case ITEM_TYPE_ITEMSPECIAL:
            e = new ItemSpecial();
            break;

        case ITEM_TYPE_TIME_ACTIVATED:
            e = new ItemTimeActivated();
            break;

        case ITEM_TYPE_ARMOR:
            e = new ItemArmor();
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
            case ITEM_TYPE_ITEM:
                return this->qty < b.qty;

            case ITEM_TYPE_EQUIPABLE:
                return *dynamic_cast<ItemEquipable*>(this) < dynamic_cast<ItemEquipable&>(b);

            case ITEM_TYPE_WEAPON:
                return *dynamic_cast<ItemWeapon*>(this) < dynamic_cast<ItemWeapon&>(b);

            case ITEM_TYPE_RANGED:
                return *dynamic_cast<ItemRanged*>(this) < dynamic_cast<ItemRanged&>(b);

            case ITEM_TYPE_COMBAT_SPELL:
                return *dynamic_cast<ItemCombatSpell*>(this) < dynamic_cast<ItemCombatSpell&>(b);

            case ITEM_TYPE_POTION:
                return *dynamic_cast<ItemPotion*>(this) < dynamic_cast<ItemPotion&>(b);

            case ITEM_TYPE_UTILITY_SPELL:
                return *dynamic_cast<ItemUtilitySpell*>(this) < dynamic_cast<ItemUtilitySpell&>(b);

            case ITEM_TYPE_ITEMSPECIAL:
                return *dynamic_cast<ItemSpecial*>(this) < dynamic_cast<ItemSpecial&>(b);

            case ITEM_TYPE_TIME_ACTIVATED:
                return *dynamic_cast<ItemTimeActivated*>(this) < dynamic_cast<ItemTimeActivated&>(b);

            case ITEM_TYPE_ARMOR:
                return *dynamic_cast<ItemArmor*>(this) < dynamic_cast<ItemArmor&>(b);
        }
    }
    return this->getItemTypeId() < b.getItemTypeId();
}

//
//  UtiitySpell.h
//  Underneath
//
//  Created by Braeden Atlee on 3/17/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UtiitySpell__
#define __Underneath__UtiitySpell__

#include "Global.hpp"
#include "Item.hpp"
#include "Math.hpp"

typedef int SpellEffect;

const SpellEffect spellRemoteUse = 0;
const SpellEffect spellRelocate = 1;
const SpellEffect spellBarrier = 2;

const SpellEffect spellDebugPlaceWall = 101;
const SpellEffect spellDebugPlaceFloor = 102;
const SpellEffect spellDebugPlaceGoblin = 103;
const SpellEffect spellDebugPlaceShop = 104;

class ItemUtilitySpell : public Item {
public:

    static ItemUtilitySpell* cloneUnsafe(ItemUtilitySpell* oldE, ItemUtilitySpell* newE = nullptr);

    ItemUtilitySpell() : Item() {

    }

    ItemUtilitySpell(SpellEffect spellEffect, int manaCost, string name) : Item(name) {
        this->spellEffect = spellEffect;
        this->manaCost = manaCost;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_UTILITY_SPELL;
    }

    virtual UseType getUseType(){
        return useInWorld;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemUtilitySpell* otherW = dynamic_cast<ItemUtilitySpell*> (other);
        return (otherW)
        		&& Item::equalsExceptQty(other)
                && (spellEffect == otherW->spellEffect)
        		&& Math::aproxEqual(manaCost, otherW->manaCost)
        		&& (continuousUse == otherW->continuousUse);
    }

    SpellEffect spellEffect = 0;
    double manaCost = 0; // -1 means one use without mana cost
    bool continuousUse = false;

    virtual string getExtendedName() {
        return name;
    }

    bool operator< (ItemUtilitySpell& b){
        return this->spellEffect < b.spellEffect;
    }

protected:
};

#endif /* defined(__Underneath__UtiitySpell__) */

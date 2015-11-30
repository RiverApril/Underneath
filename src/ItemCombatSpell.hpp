//
//  ItemCombatSpell.h
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemCombatSpell__
#define __Underneath__ItemCombatSpell__

#include "Global.hpp"
#include "ItemRanged.hpp"

class ItemCombatSpell : public ItemRanged {
public:

    static ItemCombatSpell* cloneUnsafe(ItemCombatSpell* oldE, ItemCombatSpell* newE);

    ItemCombatSpell() : ItemRanged() {

    }

    ItemCombatSpell(int baseDamage, string name, int manaCost, double useDelay, int range) : ItemRanged(baseDamage, name, useDelay, range) {

        this->baseDamage = baseDamage;
        this->damageType = damFire;
        this->manaCost = manaCost;
        this->durability = INFINITY;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_COMBAT_SPELL;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemCombatSpell* otherS = dynamic_cast<ItemCombatSpell*> (other);
        return ItemRanged::equalsExceptQty(other)
                &&(otherS)
                &&(manaCost == otherS->manaCost);
    }

    int manaCost = 0;

    bool operator< (ItemCombatSpell& b){
        return this->damageType < b.damageType;
    }

protected:

};

#endif /* defined(__Underneath__ItemCombatSpell__) */

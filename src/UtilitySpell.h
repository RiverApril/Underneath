//
//  UtiitySpell.h
//  Underneath
//
//  Created by Braeden Atlee on 3/17/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UtiitySpell__
#define __Underneath__UtiitySpell__

#include "Global.h"
#include "Item.h"

typedef int SpellEffect;

const SpellEffect spellRemoteUse = 0;
const SpellEffect spellRelocate = 1;

class UtilitySpell : public Item {
public:

    static UtilitySpell* cloneUnsafe(UtilitySpell* oldE, UtilitySpell* newE = nullptr);

    UtilitySpell() : Item() {

    }

    UtilitySpell(SpellEffect spellEffect, int manaCost, string name, double weight) : Item(name, weight) {
        this->spellEffect = spellEffect;
        this->manaCost = manaCost;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_UTILITY_SPELL;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        UtilitySpell* otherW = dynamic_cast<UtilitySpell*> (other);
        return Item::equalsExceptQty(other)
                && (otherW)
                && (spellEffect == otherW->spellEffect)
                && (manaCost == otherW->manaCost);
    }

    virtual bool instantUse() {
        return !(
                (spellEffect == spellRemoteUse) || (spellEffect == spellRelocate)
                );
    }

    SpellEffect spellEffect = 0;
    int manaCost = 0; // -1 means one use without mana cost

    virtual string getExtendedName() {
        return name;
    }

protected:
};

#endif /* defined(__Underneath__UtiitySpell__) */

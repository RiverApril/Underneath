//
//  UtiitySpell.hpp
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

enum SpellEffect{spellRemoteUse, spellRelocate, spellBarrier, spellDebugPlaceWall, spellDebugPlaceFloor, spellDebugPlaceGoblin, spellDebugPlaceShop, spellDebugPlaceBoss, spellSwap};

class ItemUtilitySpell : public Item {
public:

    ItemUtilitySpell() : Item() {

    }

    ItemUtilitySpell(SpellEffect spellEffect, int manaCost, string name) : Item(name) {
        this->spellEffect = spellEffect;
        this->manaCost = manaCost;
    }

    virtual int getItemTypeId() {
        return itemTypeUtilitySpell;
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
        		&& aproxEqual(manaCost, otherW->manaCost)
        		&& (continuousUse == otherW->continuousUse);
    }

    SpellEffect spellEffect = spellRelocate;
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

//
//  ItemCombatSpell.hpp
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemCombatSpell__
#define __Underneath__ItemCombatSpell__

#include "Global.hpp"
#include "ItemRanged.hpp"
#include "Math.hpp"

class ItemCombatSpell : public ItemRanged {
public:

    ItemCombatSpell() : ItemRanged() {

    }

    ItemCombatSpell(int baseDamage, string name, int manaCost, double useDelay, int range) : ItemRanged(baseDamage, name, useDelay, range) {
        
        this->weaponType = wepMagic;
        this->baseDamage = baseDamage;
        this->damageType = damFire;
        this->manaCost = manaCost;
        //this->durability = -1;
    }

    virtual int getItemTypeId() {
        return itemTypeCombatSpell;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemCombatSpell* otherS = dynamic_cast<ItemCombatSpell*> (other);
        return (otherS)
        		&& ItemRanged::equalsExceptQty(other)
        		&& aproxEqual(manaCost, otherS->manaCost);
    }

    double manaCost = 0;

    bool operator< (ItemCombatSpell& b){
        return this->damageType < b.damageType;
    }

protected:

};

#endif /* defined(__Underneath__ItemCombatSpell__) */

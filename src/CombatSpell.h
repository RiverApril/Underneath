//
//  CombatSpell.h
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__CombatSpell__
#define __Underneath__CombatSpell__

#include "Global.h"
#include "Ranged.h"

class CombatSpell : public Ranged {
public:

    static CombatSpell* cloneUnsafe(CombatSpell* oldE, CombatSpell* newE);

    CombatSpell() : Ranged() {

    }

    CombatSpell(int baseDamage, string name, double weight, int manaCost, double useDelay, int range) : Ranged(baseDamage, name, weight, useDelay, range) {

        this->baseDamage = baseDamage;
        this->damageType = damFire;
        this->manaCost = manaCost;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_COMBAT_SPELL;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        CombatSpell* otherS = dynamic_cast<CombatSpell*> (other);
        return Ranged::equalsExceptQty(other)
                &&(otherS)
                &&(manaCost == otherS->manaCost);
    }

    int manaCost = 0;

protected:

};

#endif /* defined(__Underneath__CombatSpell__) */

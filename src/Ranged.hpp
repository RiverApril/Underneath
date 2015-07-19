//
//  Ranged.h
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Ranged__
#define __Underneath__Ranged__

#include "Global.hpp"
#include "ItemWeapon.hpp"

class Ranged : public ItemWeapon {
public:

    static Ranged* cloneUnsafe(Ranged* oldE, Ranged* newE);

    Ranged() : ItemWeapon() {

    }

    Ranged(double baseDamage, string name, double weight, double useDelay, double range) : ItemWeapon(baseDamage, name, weight, useDelay) {
        this->baseDamage = baseDamage;
        this->damageType = damPierce;
        this->range = range;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_RANGED;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        Ranged* otherR = dynamic_cast<Ranged*> (other);
        return ItemWeapon::equalsExceptQty(other)
                &&(otherR)
                &&(range == otherR->range);
    }

    double range = 4;

protected:
};

#endif /* defined(__Underneath__Ranged__) */

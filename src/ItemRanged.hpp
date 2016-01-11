//
//  ItemRanged.hpp
//  Underneath
//
//  Created by Braeden Atlee on 1/22/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemRanged__
#define __Underneath__ItemRanged__

#include "Global.hpp"
#include "ItemWeapon.hpp"

class ItemRanged : public ItemWeapon {
public:

    static ItemRanged* cloneUnsafe(ItemRanged* oldE, ItemRanged* newE);

    ItemRanged() : ItemWeapon() {

    }

    ItemRanged(double baseDamage, string name, double useDelay, double range) : ItemWeapon(baseDamage, name, useDelay) {
        this->baseDamage = baseDamage;
        this->damageType = damPierce;
        this->range = range;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_RANGED;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemRanged* otherR = dynamic_cast<ItemRanged*> (other);
        return (otherR)
        		&& ItemWeapon::equalsExceptQty(other)
                && aproxEqual(range, otherR->range);
    }

    double range = 4;

    bool operator< (ItemRanged& b){
        return this->damageType < b.damageType;
    }

protected:
};

#endif /* defined(__Underneath__ItemRanged__) */

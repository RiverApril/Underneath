//
//  ItemRanged.hpp
//  Underneath
//
//  Created by Emily Atlee on 1/22/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__ItemRanged__
#define __Underneath__ItemRanged__

#include "Global.hpp"
#include "ItemWeapon.hpp"

enum RangedType{
    rangedUnlimited, rangedOneUse, rangedOneUseRecoverable
};

class ItemRanged : public ItemWeapon {
public:

    ItemRanged() : ItemWeapon() {

    }

    ItemRanged(double baseDamage, string name, double useDelay, double range) : ItemWeapon(baseDamage, name, useDelay) {
        this->weaponType = wepRanged;
        this->baseDamage = baseDamage;
        this->damageType = damPierce;
        this->range = range;
    }

    virtual int getItemTypeId() {
        return itemTypeRanged;
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
    RangedType rangedType;


    bool operator< (ItemRanged& b){
        if(this->damageType != b.damageType){
            return this->damageType < b.damageType;
        }
        return this->name < b.name;
    }

protected:
};

#endif /* defined(__Underneath__ItemRanged__) */

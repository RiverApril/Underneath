//
//  ItemExplosive.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemExplosive__
#define __Underneath__ItemExplosive__

#include "Global.hpp"
#include "Item.hpp"

enum ExplosiveType{dudBomb, timeActivatedBomb, pressureBomb, throwableBomb};


class ItemExplosive : public Item {
public:


    static ItemExplosive* cloneUnsafe(ItemExplosive* oldE, ItemExplosive* newE = nullptr);

    ItemExplosive() : Item(){

    }

    ItemExplosive(string name, ExplosiveType explosiveType, double time, double power, double radius, bool destroysTiles);

    virtual int getItemTypeId(){
        return ITEM_TYPE_TIME_ACTIVATED;
    }

    virtual UseType getUseType();

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    ExplosiveType explosiveType = timeActivatedBomb;
    double time = 1;
    double power = 1;
    double radius = 4;
    bool destroysTiles = false;

    bool operator< (ItemExplosive& b){
        return this->explosiveType < b.explosiveType;
    }


protected:
};


#endif /* defined(__Underneath__ItemExplosive__) */
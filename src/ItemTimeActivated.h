//
//  ItemTimeActivated.h
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemTimeActivated__
#define __Underneath__ItemTimeActivated__

#include "Global.h"
#include "Item.h"

typedef int TimeActivatedType;
const TimeActivatedType timeActivatedBomb = 0;
const TimeActivatedType timeActivatedWallBomb = 1;


class ItemTimeActivated : public Item {
public:


    static ItemTimeActivated* cloneUnsafe(ItemTimeActivated* oldE, ItemTimeActivated* newE = nullptr);

    ItemTimeActivated() : Item(){

    }

    ItemTimeActivated(TimeActivatedType timeActivatedType, double time, double power, double radius, double weight);

    virtual int getItemTypeId(){
        return ITEM_TYPE_TIME_ACTIVATED;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    TimeActivatedType timeActivatedType = 0;
    double time = 1;
    double power = 1;
    double radius = 4;


protected:
};


#endif /* defined(__Underneath__ItemTimeActivated__) */

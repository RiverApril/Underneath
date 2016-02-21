//
//  ItemTimeActivated.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/11/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemTimeActivated__
#define __Underneath__ItemTimeActivated__

#include "Global.hpp"
#include "Item.hpp"

enum TimeActivatedType{timeActivatedBomb, timeActivatedWallBomb, timeActivatedDud};


class ItemTimeActivated : public Item {
public:


    static ItemTimeActivated* cloneUnsafe(ItemTimeActivated* oldE, ItemTimeActivated* newE = nullptr);

    ItemTimeActivated() : Item(){

    }

    ItemTimeActivated(TimeActivatedType timeActivatedType, double time, double power, double radius);

    virtual int getItemTypeId(){
        return ITEM_TYPE_TIME_ACTIVATED;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    TimeActivatedType timeActivatedType = timeActivatedDud;
    double time = 1;
    double power = 1;
    double radius = 4;

    bool operator< (ItemTimeActivated& b){
        return this->timeActivatedType < b.timeActivatedType;
    }


protected:
};


#endif /* defined(__Underneath__ItemTimeActivated__) */

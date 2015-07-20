//
//  ItemSpecial.h
//  Underneath
//
//  Created by Braeden Atlee on 4/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemSpecial__
#define __Underneath__ItemSpecial__

#include "Global.hpp"
#include "Item.hpp"
#include "Abilities.hpp"

typedef int SpecialType;
const SpecialType specialtyCompass = 0;


class ItemSpecial : public Item {
public:


    static ItemSpecial* cloneUnsafe(ItemSpecial* oldE, ItemSpecial* newE = nullptr);

    ItemSpecial() : Item(){

    }

    ItemSpecial(SpecialType specialty);

    virtual int getItemTypeId(){
        return ITEM_TYPE_ITEMSPECIAL;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    SpecialType specialty = 0;

    bool operator< (ItemSpecial& b){
        return this->specialty < b.specialty;
    }
    
    
protected:
};

#endif /* defined(__Underneath__ItemSpecial__) */

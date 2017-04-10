//
//  ItemSpecial.hpp
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

enum SpecialType{specialtyKey, specialtyRepairer, specialtyCompass, specialtyCoin};


class ItemSpecial : public Item {
public:

    ItemSpecial() : Item(){

    }

    ItemSpecial(SpecialType specialty);

    virtual int getItemTypeId(){
        return itemTypeSpecial;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    SpecialType specialty = specialtyCoin;

    bool operator< (ItemSpecial& b){
        return this->specialty < b.specialty;
    }

    virtual UseType getUseType(){
        switch (specialty) {
            case specialtyRepairer:
                return useInInventory;
                
            default:
                return useInstant;
        }
    }
    
    
protected:
};

#endif /* defined(__Underneath__ItemSpecial__) */

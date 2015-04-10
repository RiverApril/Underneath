//
//  Equipable.h
//  Underneath
//
//  Created by Braeden Atlee on 3/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Equipable__
#define __Underneath__Equipable__

#include "Global.h"
#include "Item.h"
#include "Abilities.h"

typedef int EquipSlot;

const EquipSlot slotNone = 0;
const EquipSlot slotHand = 1;
const EquipSlot slotHead = 2;
const EquipSlot slotBody = 3;
const EquipSlot slotFeet = 4;
const EquipSlot slotFinger = 5;

class Equipable : public Item {
public:


    static Equipable* cloneUnsafe(Equipable* oldE, Equipable* newE = nullptr);

    Equipable() : Item() {

    }

    Equipable(string name, double weight);

    virtual int getItemTypeId() {
        return ITEM_TYPE_EQUIPABLE;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    virtual bool canBeEquipedHere(EquipSlot e) {
        return false;
    }

    Abilities<int> minimumAbilities = Abilities<int>(0);


protected:
};

#endif /* defined(__Underneath__Equipable__) */

//
//  Item.h
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Item__
#define __Underneath__Item__

#include "Global.hpp"
#include "Art.hpp"

class Level;

const int ITEM_TYPE_ITEM = 0;
const int ITEM_TYPE_EQUIPABLE = 1;
const int ITEM_TYPE_WEAPON = 2;
const int ITEM_TYPE_RANGED = 3;
const int ITEM_TYPE_COMBAT_SPELL = 4;
const int ITEM_TYPE_ARMOR = 5;
const int ITEM_TYPE_POTION = 6;
const int ITEM_TYPE_UTILITY_SPELL = 7;
const int ITEM_TYPE_ITEMSPECIAL = 8;
const int ITEM_TYPE_TIME_ACTIVATED = 9;

class Item {
public:

    static Item* clone(Item* oldI);

    Item() {

    }

    Item(string name, double weight, int qty = 1) {
        this->name = name;
        this->weight = weight;
        this->qty = qty;
    }

    virtual ~Item() {

    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId() {
        return ITEM_TYPE_ITEM;
    }

    static Item* loadNew(unsigned char* data, int* position);

    bool equals(Item* other) {
        return equalsExceptQty(other) && other->qty == qty;
    }

    virtual bool instantUse() {
        return true;
    }

    virtual bool equalsExceptQty(Item* other) {
        return other
                && getItemTypeId() == other->getItemTypeId()
                &&(name.compare(other->name) == 0)
                &&(weight == other->weight)
                &&(artIndex == other->artIndex)
                &&(coinValue == other->coinValue);
    }

    double weight = 0;
    int qty = 1;
    int coinValue = 0;

    int artIndex = -1;

    string name;

    bool operator< (Item& b){
        return this->getItemTypeId() < b.getItemTypeId();
    }


protected:

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

    static Item* cloneUnsafe(Item* oldE, Item* newE);

};

template <typename T> bool comparePointer(T* a, T* b){
    return *a < *b;
}

#endif /* defined(__Underneath__Item__) */

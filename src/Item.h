//
//  Item.h
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Item__
#define __Underneath__Item__

#include "Global.h"
#include "Art.h"

typedef double Weight;

class Level;

const int ITEM_TYPE_ITEM = 0;
const int ITEM_TYPE_WEAPON = 1;
const int ITEM_TYPE_RANGED = 2;
const int ITEM_TYPE_SPELL = 3;

class Item {
public:

    static Item* clone(Item* oldI);

    Item() : Item("UNDEFINED", 0){

    }

    Item(string name, Weight weight, int qty = 1){
        this->name = name;
        this->weight = weight;
        this->qty = qty;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId(){
        return ITEM_TYPE_ITEM;
    }

    static Item* loadNew(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other){
        return (name.compare(other->name)==0)
        &&(weight == other->weight)
        &&(artIndex == other->artIndex);
    }

    string name;
    Weight weight;
    int qty;

    int artIndex = -1;


protected:

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

    static Item* cloneUnsafe(Item* oldE, Item* newE);

};

#endif /* defined(__Underneath__Item__) */

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

typedef double Weight;

static Weight weightPaper = .2;
static Weight weightSpell = .5;

class Level;

const int ITEM_TYPE_ITEM = 0;
const int ITEM_TYPE_WEAPON = 1;
const int ITEM_TYPE_SPELL = 2;

class Item {
public:

    static Item* clone(Item* oldE, Item* newE);

    Item() : Item("UNDEFINED", 0){
        
    }

    Item(string name, Weight weight){
        this->name = name;
        this->weight = weight;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId();

    static Item* loadNew(unsigned char* data, int* position);

    string name;
    Weight weight;

};

#endif /* defined(__Underneath__Item__) */

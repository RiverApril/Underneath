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

class Level;

const int ITEM_TYPE_ITEM = 0;
const int ITEM_TYPE_WEAPON = 1;

class Item {
public:

    static Item* clone(Item* oldE, Item* newE);

    Item() : Item("UNDEFINED"){
        
    }

    Item(string name){
        this->name = name;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId();

    static Item* loadNew(unsigned char* data, int* position);

    string name;

};

#endif /* defined(__Underneath__Item__) */

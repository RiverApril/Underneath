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

class Item : public enable_shared_from_this<Item>{
public:

    Item() : Item("UNDEFINED"){
        
    }

    Item(string name){
        this->name = name;
    }

    virtual void save(string* data);

    virtual void load(char* data, int* position);

    virtual int getItemTypeId();

    static shared_ptr<Item> loadNew(char* data, int* position);

    string name;

};

#endif /* defined(__Underneath__Item__) */

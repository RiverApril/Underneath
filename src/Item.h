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
const int ITEM_TYPE_MATERIAL_ITEM = 1;
const int ITEM_TYPE_WEAPON = 2;
const int ITEM_TYPE_RANGED = 3;
const int ITEM_TYPE_SPELL = 4;
const int ITEM_TYPE_POTION = 5;

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

    virtual ~Item(){
        
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId(){
        return ITEM_TYPE_ITEM;
    }

    static Item* loadNew(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other){
        return getItemTypeId()==other->getItemTypeId()
        &&(name.compare(other->name)==0)
        &&(weight == other->weight)
        &&(artIndex == other->artIndex);
    }

    virtual string getExtendedName(){
        return name;
    }

    virtual void setBaseName(string baseName){
        name = baseName;
    }

    Weight weight;
    int qty;

    int artIndex = -1;


protected:

    string name;

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

    static Item* cloneUnsafe(Item* oldE, Item* newE);

};

#endif /* defined(__Underneath__Item__) */

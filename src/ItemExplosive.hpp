//
//  ItemExplosive.hpp
//  Underneath
//
//  Created by Emily Atlee on 4/11/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__ItemExplosive__
#define __Underneath__ItemExplosive__

#include "Global.hpp"
#include "Item.hpp"
#include "Icon.hpp"

enum ExplosiveType{dudBomb, timeActivatedBomb, pressureBomb, throwableBomb};


class ItemExplosive : public Item {
public:
    

    ItemExplosive() : Item(){

    }

    ItemExplosive(string name, ExplosiveType explosiveType, double time, double power, double radius, bool destroysTiles);

    virtual int getItemTypeId(){
        return itemTypeExplosive;
    }

    virtual UseType getUseType();

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    ExplosiveType explosiveType = timeActivatedBomb;
    double time = 1;
    double power = 1;
    double radius = 4;
    bool destroysTiles = false;
    int iconIndex = -1;

    bool operator< (ItemExplosive& b){
        if(this->explosiveType != b.explosiveType){
            return this->explosiveType < b.explosiveType;
        }
        return this->name < b.name;
    }


protected:
};


#endif /* defined(__Underneath__ItemExplosive__) */

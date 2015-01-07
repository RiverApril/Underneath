//
//  Spell.h
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Spell__
#define __Underneath__Spell__

#include "Global.h"
#include "Item.h"


class Spell : public Item{
public:

    static Spell* clone(Spell* oldE, Spell* newE);


    Spell() : Spell(0, "UNDEFINED", 0, 0, 0){

    }

    Spell(int baseDamage, string name, Weight weight, int manaCost, double castDelay) : Item(name, weight){
        this->baseDamage = baseDamage;
        this->manaCost = manaCost;
        this->castDelay = castDelay;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId();

    int baseDamage = 0;
    int manaCost = 0;
    double castDelay = 1;
};

#endif /* defined(__Underneath__Spell__) */

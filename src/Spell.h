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
#include "Ranged.h"


class Spell : public Ranged {
public:

    static Spell* clone(Spell* oldE, Spell* newE);


    Spell() : Spell(0, "UNDEFINED", 0, 0, 0, 0){

    }

    Spell(int baseDamage, string name, Weight weight, int manaCost, double useDelay, int range) : Ranged(baseDamage, name, weight, useDelay, range){
        this->baseDamage = baseDamage;
        this->damageType = damMagic;
        this->manaCost = manaCost;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId();

    virtual bool equalsExceptQty(Item* other){
        Spell* otherS = dynamic_cast<Spell*>(other);
        return Ranged::equalsExceptQty(other)
        &&(otherS)
        &&(manaCost == otherS->manaCost);
    }

    int manaCost = 0;
};

#endif /* defined(__Underneath__Spell__) */

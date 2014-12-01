//
//  Weapon.h
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Weapon__
#define __Underneath__Weapon__

#include "Global.h"
#include "Item.h"


typedef int EnchantmentId;

const EnchantmentId enchFire = 0;
const EnchantmentId enchBleed = 1;


struct Enchantment{
    Enchantment(EnchantmentId eId, int chance, int power){
        this->eId = eId;
        this->chance = chance;
        this->power = power;
    }
    EnchantmentId eId = enchFire;
    int chance = 1;
    int power = 1;
};




typedef int DamageType;


class Weapon : public Item{
public:

    static Weapon* clone(Weapon* oldE, Weapon* newE);


    Weapon() : Weapon(0, "UNDEFINED"){

    }

    Weapon(int baseDamage, string name) : Item(name){
        this->baseDamage = baseDamage;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual int getItemTypeId();

    Weapon* addEnchantment(EnchantmentId eId, int chance, int power){
        enchantments.push_back(Enchantment(eId,chance ,power));
        return this;
    }

    Weapon* addEnchantment(Enchantment e){
        enchantments.push_back(e);
        return this;
    }

    int baseDamage = 1;
    vector<Enchantment> enchantments;
};

#endif /* defined(__Underneath__Weapon__) */

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

static Weight weightKnife = 1;
static Weight weightSmallSword = 2;
static Weight weightLargeSword = 4;

static Weight weightSpear = 3;

static Weight weightMace = 3;
static Weight weightClub = 6;


typedef int DamageType;
const DamageType damMelee = 0;
const DamageType damRanged = 1;
const DamageType damMagic = 2;


class Weapon : public Item{
public:

    static Weapon* clone(Weapon* oldE, Weapon* newE);


    Weapon() : Weapon(0, "UNDEFINED", 1, damMelee){

    }

    Weapon(int baseDamage, string name, Weight weight, DamageType damageType) : Item(name, weight){
        this->baseDamage = baseDamage;
        this->damageType = damageType;
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
    DamageType damageType = damMelee;
};

#endif /* defined(__Underneath__Weapon__) */

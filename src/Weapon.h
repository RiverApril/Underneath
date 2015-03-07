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
#include "Enchantment.h"

typedef int DamageType;
const DamageType damSharp = 0;
const DamageType damBlunt = 1;
const DamageType damPierce = 2;
const DamageType damFire = 3;
const DamageType damIce = 4;
const DamageType damShock = 5;
const DamageType damPoison = 6;
const DamageType damBlood = 7;

static string damageTypeToString(DamageType d){
    switch(d){
        case damSharp:
            return "Sharp";
        case damBlunt:
            return "Blunt";
        case damFire:
            return "Fire";
        case damIce:
            return "Ice";
        case damShock:
            return "Shock";
        case damPoison:
            return "Poison";
        case damBlood:
            return "Blood";
        default:
            return "Undefined";
    }
}


class Weapon : public Item {
public:


    static Weapon* cloneUnsafe(Weapon* oldE, Weapon* newE = nullptr);

    Weapon() : Item(){

    }

    Weapon(double baseDamage, string name, double weight, double useDelay);

    virtual int getItemTypeId(){
        return ITEM_TYPE_WEAPON;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    Weapon* addEnchantment(EnchantmentId eId, int chance, int power){
        enchantments.push_back(Enchantment(eId, chance ,power));
        return this;
    }

    Weapon* addEnchantment(Enchantment e){
        enchantments.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other){
        Weapon* otherW = dynamic_cast<Weapon*>(other);
        return Item::equalsExceptQty(other)
        &&(otherW)
        &&(baseDamage == otherW->baseDamage)
        &&(damageType == otherW->damageType)
        &&(enchantments == otherW->enchantments)
        &&(useDelay == otherW->useDelay);
    }

    double baseDamage = 1;
    DamageType damageType = damSharp;
    double useDelay = 1;
    
    vector<Enchantment> enchantments;

protected:
};

#endif /* defined(__Underneath__Weapon__) */

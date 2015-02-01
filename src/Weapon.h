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

typedef int DamageType;
const DamageType damMelee = 0;
const DamageType damRanged = 1;
const DamageType damMagic = 2;


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

inline bool operator==(const Enchantment& a, const Enchantment& b){
    return (a.eId == b.eId)&&(a.chance==b.chance)&&(a.power==b.power);
}

inline bool operator!=(const Enchantment& a, const Enchantment& b){
    return !(a == b);
}


class Weapon : public Item {
public:

    static string damageTypeToString(DamageType d){
        switch(d){
            case damMelee:
                return "Melee";
            case damRanged:
                return "Ranged";
            case damMagic:
                return "Magic";
            default:
                return "Undefined";
        }
    }

    static string enchantmentIdToString(EnchantmentId e){
        switch(e){
            case enchFire:
                return "Fire";
            case enchBleed:
                return "Bleed";
            default:
                return "Undefined";
        }
    }


    static Weapon* cloneUnsafe(Weapon* oldE, Weapon* newE = nullptr);

    Weapon() : Weapon(0, "UNDEFINED", 0, 0){

    }

    Weapon(double baseDamage, string name, Weight weight, double useDelay) : Item(name, weight){
        this->baseDamage = baseDamage;
        this->useDelay = useDelay;

        this->damageType = damMelee;
    }

    virtual int getItemTypeId(){
        return ITEM_TYPE_WEAPON;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    Weapon* addEnchantment(EnchantmentId eId, int chance, int power){
        enchantments.push_back(Enchantment(eId,chance ,power));
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
    DamageType damageType = damMelee;
    double useDelay = 1;
    
    vector<Enchantment> enchantments;

protected:
};

#endif /* defined(__Underneath__Weapon__) */

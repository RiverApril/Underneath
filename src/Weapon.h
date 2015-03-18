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
#include "Equipable.h"
#include "Enchantment.h"
#include "Ui.h"

typedef int WeaponType;

const WeaponType wepMelee = 0;
const WeaponType wepRanged = 1;
const WeaponType wepMagic = 2;

typedef int DamageType;
const DamageType damSharp = 0;
const DamageType damBlunt = 1;
const DamageType damPierce = 2;
const DamageType damFire = 3;
const DamageType damIce = 4;
const DamageType damShock = 5;
const DamageType damPoison = 6;
const DamageType damBlood = 7;
const DamageType damDebug = 8;


class Weapon : public Equipable {
public:

    static string damageTypeName(DamageType d){
        switch(d){
            case damSharp:
                return "Sharp";
            case damBlunt:
                return "Blunt";
            case damPierce:
                return "Pierce";
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
            case damDebug:
                return "Debug";
            default:
                return "Undefined";
        }
    }

    static Ui::Color damageTypeColor(DamageType d){
        switch(d){
            case damSharp:
                return Ui::C_LIGHT_GRAY;
            case damBlunt:
                return Ui::C_LIGHT_GRAY;
            case damFire:
                return Ui::C_LIGHT_YELLOW;
            case damIce:
                return Ui::C_LIGHT_CYAN;
            case damShock:
                return Ui::C_LIGHT_MAGENTA;
            case damPoison:
                return Ui::C_LIGHT_GREEN;
            case damBlood:
                return Ui::C_LIGHT_RED;
            case damDebug:
                return Ui::C_WHITE;
            default:
                return Ui::C_WHITE;
        }
    }

    static string effectName(EffectId eid, double meta){
        switch (eid) {
            case effDamage:
                return damageTypeName((DamageType)meta);

            case effHeal:
                return "Heal";

            case effBuff:
                return "Buff TODO";

            default:
                return "UNDEFINED";
        }
    }
    
    static string enchantmentName(Enchantment e){
        return effectName(e.effectId, e.meta);
    }


    static Weapon* cloneUnsafe(Weapon* oldE, Weapon* newE = nullptr);

    Weapon() : Equipable(){

    }

    Weapon(double baseDamage, string name, double weight, double useDelay);

    virtual int getItemTypeId(){
        return ITEM_TYPE_WEAPON;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool instantUse(){
        return false;
    }

    Weapon* addEnchantment(Enchantment e){
        enchantments.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other){
        Weapon* otherW = dynamic_cast<Weapon*>(other);
        return Equipable::equalsExceptQty(other)
        &&(otherW)
        &&(baseDamage == otherW->baseDamage)
        &&(damageType == otherW->damageType)
        &&(enchantments == otherW->enchantments)
        &&(useDelay == otherW->useDelay);
    }

    virtual bool canBeEquipedHere(EquipSlot e){
        return e==slotWeapon;
    }

    double baseDamage = 1;
    DamageType damageType = damSharp;
    WeaponType weaponType = wepMelee;
    double useDelay = 1;
    
    vector<Enchantment> enchantments;

protected:
};

#endif /* defined(__Underneath__Weapon__) */

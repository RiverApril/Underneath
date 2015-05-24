//
//  Weapon.h
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Weapon__
#define __Underneath__Weapon__

#include "Global.hpp"
#include "Equipable.hpp"
#include "Enchantment.hpp"
#include "Ui.hpp"
#include "DamageType.hpp"

const vector<EquipSlot> weaponSlots = {slotWep1, slotWep2};

class Weapon : public Equipable {
public:


    static Weapon* cloneUnsafe(Weapon* oldE, Weapon* newE = nullptr);

    Weapon() : Equipable() {

    }

    Weapon(double baseDamage, string name, double weight, double useDelay);

    virtual int getItemTypeId() {
        return ITEM_TYPE_WEAPON;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool instantUse() {
        return false;
    }

    Weapon* addEnchantment(Enchantment e) {
        enchantments.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other) {
        Weapon* otherW = dynamic_cast<Weapon*> (other);
        return Equipable::equalsExceptQty(other)
                &&(otherW)
                &&(baseDamage == otherW->baseDamage)
                &&(damageType == otherW->damageType)
                &&(enchantments == otherW->enchantments)
        		&&(useDelay == otherW->useDelay);
    }

    virtual vector<EquipSlot> getViableSlots(){
        return weaponSlots;
    }

    double baseDamage = 1;
    DamageType damageType = damSharp;
    WeaponType weaponType = wepMelee;
    double useDelay = 1;

    vector<Enchantment> enchantments;

protected:
};

#endif /* defined(__Underneath__Weapon__) */

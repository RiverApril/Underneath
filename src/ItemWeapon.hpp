//
//  ItemWeapon.hpp
//  Underneath
//
//  Created by Emily Atlee on 11/11/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__ItemWeapon__
#define __Underneath__ItemWeapon__

#include "Global.hpp"
#include "ItemEquipable.hpp"
#include "Enchantment.hpp"
#include "Ui.hpp"
#include "DamageType.hpp"
#include "Math.hpp"

const vector<EquipSlot> weaponSlots = {slotWep1, slotWep2};

class ItemWeapon : public ItemEquipable {
public:
    

    ItemWeapon() : ItemEquipable() {

    }

    ItemWeapon(double baseDamage, string name, double useDelay);

    virtual int getItemTypeId() {
        return itemTypeWeapon;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual UseType getUseType() {
        return useInWorld;
    }

    ItemWeapon* addEnchantment(Enchantment e) {
        enchantments.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other) {
        ItemWeapon* otherW = dynamic_cast<ItemWeapon*> (other);
        return (otherW)
        		&& ItemEquipable::equalsExceptQty(otherW)
                && aproxEqual(baseDamage, otherW->baseDamage)
                && (damageType == otherW->damageType)
                && (enchantments == otherW->enchantments)
        		&& aproxEqual(useDelay, otherW->useDelay);
    }

    virtual vector<EquipSlot> getViableSlots(){
        return weaponSlots;
    }

    double baseDamage = 1;
    DamageType damageType = damSharp;
    WeaponType weaponType = wepMelee;
    double useDelay = 1;

    vector<Enchantment> enchantments;

    bool operator< (ItemWeapon& b){
        if(this->damageType != b.damageType){
            return this->damageType < b.damageType;
        }
        return this->name < b.name;
    }

protected:
};

#endif /* defined(__Underneath__ItemWeapon__) */

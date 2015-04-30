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
#include "DamageType.h"

const vector<EquipSlot> bothHands = {slotHand1, slotHand2};
const vector<EquipSlot> oneHand = {slotHand1};

class Weapon : public Equipable {
public:


    static Weapon* cloneUnsafe(Weapon* oldE, Weapon* newE = nullptr);

    Weapon() : Equipable() {

    }

    Weapon(double baseDamage, string name, double weight, double useDelay, bool twoHanded);

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
        		&&(useDelay == otherW->useDelay)
        		&&(twoHanded == otherW->twoHanded);
    }

    virtual bool canBeEquipedHere(EquipSlot e) {
        return e==slotHand1 || (twoHanded?false:e==slotHand2);
    }

    virtual bool blocksSlot(EquipSlot blockMe, EquipSlot whenHere) {
        return twoHanded?((whenHere == slotHand1 && blockMe == slotHand2)||(whenHere == slotHand2 && blockMe == slotHand1)):false;
    }

    virtual vector<EquipSlot> getViableSlots(){
        return twoHanded?bothHands:oneHand;
    }

    double baseDamage = 1;
    DamageType damageType = damSharp;
    WeaponType weaponType = wepMelee;
    double useDelay = 1;
    bool twoHanded = false;

    vector<Enchantment> enchantments;

protected:
};

#endif /* defined(__Underneath__Weapon__) */

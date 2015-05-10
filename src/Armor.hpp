//
//  Armor.h
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Armor__
#define __Underneath__Armor__

#include "Equipable.hpp"
#include "Enchantment.hpp"
#include "DamageType.hpp"

struct Defense {
    Defense(DamageType damType, double amount){
        this->damageType = damType;
        this->amount = amount;
    }
    Defense(unsigned char* data, int* position) {
        load(data, position);
    }
    void save(vector<unsigned char>* data){
        Utility::saveInt(data, damageType);
        Utility::saveDouble(data, amount);
    }
    void load(unsigned char* data, int* position){
        damageType = Utility::loadInt(data, position);
        amount = Utility::loadDouble(data, position);
    }
    
    DamageType damageType;
    double amount;
};

bool operator==(const Defense& a, const Defense& b);

bool operator!=(const Defense& a, const Defense& b);

class Armor : public Equipable {
public:


    static Armor* cloneUnsafe(Armor* oldE, Armor* newE = nullptr);

    Armor() : Equipable() {

    }

    Armor(vector<EquipSlot> viableSlots, vector<Defense> defenses, string name, double weight);

    Armor(EquipSlot viableSlot, vector<Defense> defenses, string name, double weight);

    virtual int getItemTypeId() {
        return ITEM_TYPE_ARMOR;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool instantUse() {
        return false;
    }

    Armor* addEnchantment(Enchantment e) {
        enchantments.push_back(e);
        return this;
    }

    Armor* addDefence(Defense e) {
        defenses.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other) {
        Armor* otherW = dynamic_cast<Armor*> (other);
        return Equipable::equalsExceptQty(other)
        &&(otherW)
        &&(defenses == otherW->defenses)
        &&(enchantments == otherW->enchantments)
        &&(viableSlots == otherW->viableSlots);
    }

    virtual bool canBeEquipedHere(EquipSlot e) {
        for(EquipSlot slot : viableSlots){
            if(slot == e){
                return true;
            }
        }
        return false;
    }

    virtual vector<EquipSlot> getViableSlots(){
        return viableSlots;
    }

    vector<Defense> defenses;
    
    vector<Enchantment> enchantments;

    vector<EquipSlot> viableSlots;
    
protected:
};


#endif /* defined(__Underneath__Armor__) */

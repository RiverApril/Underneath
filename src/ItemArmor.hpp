//
//  ItemArmor.hpp
//  Underneath
//
//  Created by Braeden Atlee on 4/29/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemArmor__
#define __Underneath__ItemArmor__

#include "ItemEquipable.hpp"
#include "Enchantment.hpp"
#include "DamageType.hpp"

struct Defense {
    Defense(DamageType damType, double amount){
        this->damageType = damType;
        this->amount = amount;
    }
    Defense(vector<unsigned char>* data, int* position) {
        load(data, position);
    }
    void save(vector<unsigned char>* data){
        Utility::saveInt(data, damageType);
        Utility::saveDouble(data, amount);
    }
    void load(vector<unsigned char>* data, int* position){
        damageType = (DamageType)Utility::loadInt(data, position);
        amount = Utility::loadDouble(data, position);
    }
    
    DamageType damageType;
    double amount; // incomingDamage * (1.0-amount) = actaulDamage
};

bool operator==(const Defense& a, const Defense& b);

bool operator!=(const Defense& a, const Defense& b);

class ItemArmor : public ItemEquipable {
public:


    ItemArmor() : ItemEquipable() {

    }

    ItemArmor(vector<EquipSlot> viableSlots, vector<Defense> defenses, string name);

    ItemArmor(EquipSlot viableSlot, vector<Defense> defenses, string name);

    virtual int getItemTypeId() {
        return itemTypeArmor;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    ItemArmor* addDefence(Defense e) {
        defenses.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other) {
        ItemArmor* otherW = dynamic_cast<ItemArmor*> (other);
        return (otherW)
        	&& ItemEquipable::equalsExceptQty(other)
            && (defenses == otherW->defenses)
            && (viableSlots == otherW->viableSlots);
    }

    virtual vector<EquipSlot> getViableSlots(){
        return viableSlots;
    }

    vector<Defense> defenses;

    vector<EquipSlot> viableSlots;

    bool operator< (ItemArmor& b){
        if(this->viableSlots[0] != b.viableSlots[0]){
            return this->viableSlots[0] < b.viableSlots[0];
        }
        return this->name < b.name;
    }
    
protected:
};


#endif /* defined(__Underneath__ItemArmor__) */

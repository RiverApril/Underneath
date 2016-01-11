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
        damageType = Utility::loadInt(data, position);
        amount = Utility::loadDouble(data, position);
    }
    
    DamageType damageType;
    double amount;
};

bool operator==(const Defense& a, const Defense& b);

bool operator!=(const Defense& a, const Defense& b);

class ItemArmor : public ItemEquipable {
public:


    static ItemArmor* cloneUnsafe(ItemArmor* oldE, ItemArmor* newE = nullptr);

    ItemArmor() : ItemEquipable() {

    }

    ItemArmor(vector<EquipSlot> viableSlots, vector<Defense> defenses, string name);

    ItemArmor(EquipSlot viableSlot, vector<Defense> defenses, string name);

    virtual int getItemTypeId() {
        return ITEM_TYPE_ARMOR;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    ItemArmor* addEnchantment(Enchantment e) {
        enchantments.push_back(e);
        return this;
    }

    ItemArmor* addDefence(Defense e) {
        defenses.push_back(e);
        return this;
    }

    virtual bool equalsExceptQty(Item* other) {
        ItemArmor* otherW = dynamic_cast<ItemArmor*> (other);
        return (otherW)
        	&& ItemEquipable::equalsExceptQty(other)
            && (defenses == otherW->defenses)
            && (enchantments == otherW->enchantments)
            && (viableSlots == otherW->viableSlots);
    }

    virtual vector<EquipSlot> getViableSlots(){
        return viableSlots;
    }

    vector<Defense> defenses;
    
    vector<Enchantment> enchantments;

    vector<EquipSlot> viableSlots;

    bool operator< (ItemArmor& b){
        return this->viableSlots[0] < b.viableSlots[0];
    }
    
protected:
};


#endif /* defined(__Underneath__ItemArmor__) */

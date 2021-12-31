//
//  ItemAreaOfEffectWeapon.hpp
//  Underneath
//
//  Created by Emily Atlee on 3/14/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef ItemAreaOfEffectWeapon_hpp
#define ItemAreaOfEffectWeapon_hpp

#include "ItemCombatSpell.hpp"

class ItemAreaOfEffectWeapon : public ItemCombatSpell {


public:

    ItemAreaOfEffectWeapon() : ItemCombatSpell() {

    }

    ItemAreaOfEffectWeapon(int baseDamage, string name, int manaCost, double useDelay, int range, int radius) : ItemCombatSpell(baseDamage, name, manaCost, useDelay, range) {

        this->baseDamage = baseDamage;
        this->damageType = damFire;
        this->manaCost = manaCost;
        //this->durability = -1;
    }

    virtual int getItemTypeId() {
        return itemTypeAofWeapon;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemCombatSpell* otherS = dynamic_cast<ItemCombatSpell*> (other);
        return (otherS)
        && ItemRanged::equalsExceptQty(other)
        && aproxEqual(manaCost, otherS->manaCost);
    }

    int radius = 0;

    bool operator< (ItemAreaOfEffectWeapon& b){
        if(this->damageType != b.damageType){
            return this->damageType < b.damageType;
        }
        return this->name < b.name;
    }
    
protected:

};

#endif /* ItemAreaOfEffectWeapon_hpp */

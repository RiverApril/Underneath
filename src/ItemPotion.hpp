//
//  ItemPotion.h
//  Underneath
//
//  Created by Braeden Atlee on 3/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemPotion__
#define __Underneath__ItemPotion__

#include "Global.hpp"
#include "Item.hpp"
#include "Effect.hpp"

class ItemPotion : public Item {
public:

    static ItemPotion* cloneUnsafe(ItemPotion* oldE, ItemPotion* newE = nullptr);

    ItemPotion() : Item() {

    }

    ItemPotion(string name, double weight) : Item(name, weight) {

    }

    ItemPotion(Effect effect, string name, double weight) : Item(name, weight) {
        this->effects = {effect};
    }

    ItemPotion(vector<Effect> effects, string name, double weight) : Item(name, weight) {
        this->effects = effects;
    }

    virtual int getItemTypeId() {
        return ITEM_TYPE_POTION;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemPotion* otherW = dynamic_cast<ItemPotion*> (other);
        return Item::equalsExceptQty(other)
                && (otherW)
                && (effects == otherW->effects);
    }

    vector<Effect> effects;

    virtual string getExtendedName() {
        return name;
    }

    bool operator< (ItemPotion& b){
        if(this->effects.size() == 1 && b.effects.size() == 1){
            return this->effects[0].eId < b.effects[0].eId;
        }else{
            return this->effects.size() < b.effects.size();
        }
    }

protected:
};

#endif /* defined(__Underneath__ItemPotion__) */
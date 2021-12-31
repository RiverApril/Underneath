//
//  ItemPotion.hpp
//  Underneath
//
//  Created by Emily Atlee on 3/2/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__ItemPotion__
#define __Underneath__ItemPotion__

#include "Global.hpp"
#include "Item.hpp"
#include "Effect.hpp"

class ItemPotion : public Item {
public:

    ItemPotion() : Item() {

    }

    ItemPotion(string name) : Item(name) {

    }

    ItemPotion(Effect effect, string name) : Item(name) {
        this->effects = {effect};
    }

    ItemPotion(vector<Effect> effects, string name) : Item(name) {
        this->effects = effects;
    }

    virtual int getItemTypeId() {
        return itemTypePotion;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other) {
        ItemPotion* otherW = dynamic_cast<ItemPotion*> (other);
        return (otherW)
            && Item::equalsExceptQty(other)
            && (effects == otherW->effects);
    }

    vector<Effect> effects;

    virtual string getExtendedName() {
        return name;
    }

    bool operator< (ItemPotion& b);

protected:
};

#endif /* defined(__Underneath__ItemPotion__) */

//
//  Item.hpp
//  Underneath
//
//  Created by Braeden Atlee on 11/11/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Item__
#define __Underneath__Item__

#include "Global.hpp"
#include "Art.hpp"

class Level;

const int ITEM_TYPE_ITEM = 0;
const int ITEM_TYPE_EQUIPABLE = 1;
const int ITEM_TYPE_WEAPON = 2;
const int ITEM_TYPE_RANGED = 3;
const int ITEM_TYPE_COMBAT_SPELL = 4;
const int ITEM_TYPE_ARMOR = 5;
const int ITEM_TYPE_POTION = 6;
const int ITEM_TYPE_UTILITY_SPELL = 7;
const int ITEM_TYPE_ITEMSPECIAL = 8;
const int ITEM_TYPE_TIME_ACTIVATED = 9;

enum UseType{useInstant, useInWorld, useInInventory};

enum EquipSlot{slotNone = -1, slotWep1, slotWep2, slotHands, slotHead, slotBody, slotFeet, slotLegs, slotFinger1, slotFinger2, slotFinger3, slotFinger4, slotNeck, slotFav1, slotFav2 = slotFav1+1, slotFav3 = slotFav2+1, slotFav4 = slotFav3+1, slotFav5 = slotFav4+1, slotFav6 = slotFav5+1, slotFav7 = slotFav6+1, slotFav8 = slotFav7+1, slotFav9 = slotFav8+1, slotFav0 = slotFav9+1, slotQty};

inline EquipSlot operator+(EquipSlot a, EquipSlot b){
    return static_cast<EquipSlot>(static_cast<int>(a) + static_cast<int>(b));
}
inline EquipSlot operator+(EquipSlot a, int b){
    return static_cast<EquipSlot>(static_cast<int>(a) + b);
}

class Item {
public:

    static Item* clone(Item* oldI);

    Item() {

    }

    Item(string name, int qty = 1) {
        this->name = name;
        this->qty = qty;
    }

    virtual ~Item() {

    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual int getItemTypeId() {
        return ITEM_TYPE_ITEM;
    }

    static Item* loadNew(vector<unsigned char>* data, int* position);

    bool equals(Item* other) {
        return other
        	&& other->qty == qty
        	&& equalsExceptQty(other);
    }

    virtual UseType getUseType(){
        return useInstant;
    }

    virtual bool canBeEquipedHere(EquipSlot e) {
        if(e >= slotFav1 && e <= slotFav0){
            return true;
        }
        return false;
    }

    virtual bool equalsExceptQty(Item* other) {
        return other
                && getItemTypeId() == other->getItemTypeId()
        		&& (name.compare(other->name) == 0)
        		&& (pluralName.compare(other->pluralName) == 0)
                && (artIndex == other->artIndex)
                /*&&(coinValue == other->coinValue)*/;
    }

    virtual string getName(bool plural){
        if(plural){
            if(pluralName.size() > 0){
            	return pluralName;
            }else{
                return name+"s";
            }
        }else{
            return name;
        }
    }

    virtual void setName(vector<string> names){
        if(names.size() > 0){
            name = names[0];
            if(names.size() > 1){
                pluralName = names[1];
            }
        }
    }

    virtual void prependName(string prepend){
        if(pluralName.size() > 0){
            pluralName = prepend + pluralName;
        }
        name = prepend + name;
    }
	
    int qty = 1;
    int coinValue = 0;

    int artIndex = -1;

    bool operator< (Item& b);



protected:

    string name;
    string pluralName = "";

    template<class Super, class Sub>
    static Sub* makeNewAndClone(Super* oldT);

    static Item* cloneUnsafe(Item* oldE, Item* newE);

};

template <typename T> bool comparePointer(T* a, T* b){
    return *a < *b;
}

#endif /* defined(__Underneath__Item__) */

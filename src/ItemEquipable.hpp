//
//  ItemEquipable.h
//  Underneath
//
//  Created by Braeden Atlee on 3/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemEquipable__
#define __Underneath__ItemEquipable__

#include "Global.hpp"
#include "Item.hpp"
#include "Abilities.hpp"

//#define primaryLeftHand false

typedef int EquipSlot;

const EquipSlot slotNone = 0;
const EquipSlot slotWep1 = 1;
const EquipSlot slotWep2 = 2;
const EquipSlot slotHands = 3;
const EquipSlot slotHead = 4;
const EquipSlot slotBody = 5;
const EquipSlot slotFeet = 6;
const EquipSlot slotLegs = 7;
const EquipSlot slotFinger1 = 8;
const EquipSlot slotFinger2 = 9;
const EquipSlot slotFinger3 = 10;
const EquipSlot slotFinger4 = 11;
const EquipSlot slotMAX = slotFinger4;

class ItemEquipable : public Item {
public:

    static char equipSlotAbr(EquipSlot slot){
        switch (slot) {
            case slotWep1:
                return 'E';
            case slotWep2:
                return 'e';
            case slotHands:
                return 'G';
            case slotHead:
                return 'H';
            case slotBody:
                return 'B';
            case slotLegs:
                return 'L';
            case slotFeet:
                return 'F';
            case slotFinger1:
                return 'O';
            case slotFinger2:
                return 'O';
            case slotFinger3:
                return 'O';
            case slotFinger4:
                return 'O';

            case slotNone:
                return '~';
            default:
                return '!';
        }
    }

    static string equipSlotName(EquipSlot slot){
        switch (slot) {
            case slotWep1:
                return "Primary Weapon";//primaryLeftHand?"Left Hand":"Right Hand";
            case slotWep2:
                return "Secondary Weapon";//primaryLeftHand?"Right Hand":"Left Hand";
            case slotHands:
                return "Hands";
            case slotHead:
                return "Head";
            case slotBody:
                return "Body";
            case slotLegs:
                return "Legs";
            case slotFeet:
                return "Feet";
            case slotFinger1:
                return "Left Ring Finger";
            case slotFinger2:
                return "Right Ring Finger";
            case slotFinger3:
                return "Left Middle Finger";
            case slotFinger4:
                return "Right Middle Finger";

            case slotNone:
                return "None";
            default:
                return "ERR: "+to_string(slot);
        }
    }

    static string equipSlotPreposition(EquipSlot slot){
        switch (slot) {
            case slotWep1:
            case slotWep2:
                return "as";

            case slotHands:
            case slotHead:
            case slotBody:
            case slotFeet:
            case slotLegs:
            case slotFinger1:
            case slotFinger2:
            case slotFinger3:
            case slotFinger4:
            case slotNone:
                return "on";
            default:
                return "ERR: "+to_string(slot);
        }
    }

    static string equipSlotNameGeneric(EquipSlot slot){
        switch (slot) {
            case slotWep1:
            case slotWep2:
                return "Weapon";
            case slotHands:
                return "Hands";
            case slotHead:
                return "Head";
            case slotBody:
                return "Body";
            case slotLegs:
                return "Legs";
            case slotFeet:
                return "Feet";
            case slotFinger1:
            case slotFinger2:
            case slotFinger3:
            case slotFinger4:
                return "Finger";

            case slotNone:
                return "None";
            default:
                return "ERR: "+to_string(slot);
        }
    }

    static ItemEquipable* cloneUnsafe(ItemEquipable* oldE, ItemEquipable* newE = nullptr);

    ItemEquipable() : Item() {

    }

    ItemEquipable(string name);

    virtual int getItemTypeId() {
        return ITEM_TYPE_EQUIPABLE;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    virtual bool canBeEquipedHere(EquipSlot e) {
        vector<EquipSlot> vs = getViableSlots();
        for(EquipSlot slot : vs){
            if(slot == e){
                return true;
            }
        }
        return false;
    }

    virtual bool blocksSlot(EquipSlot blockMe, EquipSlot whenHere) {
        return false;
    }

    virtual vector<EquipSlot> getViableSlots(){
        return {};
    }

    Abilities<int> minimumAbilities = Abilities<int>(0);

    bool operator< (ItemEquipable& b){
        if(this->getViableSlots().size() > 0 && b.getViableSlots().size() > 0){
            return this->getViableSlots()[0] < b.getViableSlots()[0];
        }else{
            return this->getViableSlots().size() < b.getViableSlots().size();
        }
    }

    int durability = 100;


protected:
};

#endif /* defined(__Underneath__ItemEquipable__) */

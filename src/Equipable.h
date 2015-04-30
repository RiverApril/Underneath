//
//  Equipable.h
//  Underneath
//
//  Created by Braeden Atlee on 3/15/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Equipable__
#define __Underneath__Equipable__

#include "Global.h"
#include "Item.h"
#include "Abilities.h"

#define primaryLeftHand false

typedef int EquipSlot;

const EquipSlot slotNone = 0;
const EquipSlot slotHand1 = 1;
const EquipSlot slotHand2 = 2;
const EquipSlot slotHead = 3;
const EquipSlot slotBody = 4;
const EquipSlot slotFeet = 5;
const EquipSlot slotFinger1 = 6;
const EquipSlot slotFinger2 = 7;
const EquipSlot slotFinger3 = 8;
const EquipSlot slotFinger4 = 9;
const EquipSlot slotMAX = slotFinger4;

class Equipable : public Item {
public:

    static char equipSlotAbr(EquipSlot slot){
        switch (slot) {
            case slotHand1:
                return 'E';
            case slotHand2:
                return 'E';
            case slotHead:
                return 'H';
            case slotBody:
                return 'B';
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
                return '-';
            default:
                return '!';
        }
    }

    static string equipSlotName(EquipSlot slot, bool twoHanded){
        if(twoHanded){
            return "Hands";
        }
        switch (slot) {
            case slotHand1:
                return primaryLeftHand?"Left Hand":"Right Hand";
            case slotHand2:
                return primaryLeftHand?"Right Hand":"Left Hand";
            case slotHead:
                return "Head";
            case slotBody:
                return "Body";
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

    static string equipSlotNameGeneric(EquipSlot slot, bool twoHanded){
        if(twoHanded){
            return "Both Hands";
        }
        switch (slot) {
            case slotHand1:
                return "Hand";
            case slotHand2:
                return "Hand";
            case slotHead:
                return "Head";
            case slotBody:
                return "Body";
            case slotFeet:
                return "Feet";
            case slotFinger1:
                return "Finger";
            case slotFinger2:
                return "Finger";
            case slotFinger3:
                return "Finger";
            case slotFinger4:
                return "Finger";

            case slotNone:
                return "None";
            default:
                return "ERR: "+to_string(slot);
        }
    }

    static Equipable* cloneUnsafe(Equipable* oldE, Equipable* newE = nullptr);

    Equipable() : Item() {

    }

    Equipable(string name, double weight);

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


protected:
};

#endif /* defined(__Underneath__Equipable__) */

//
//  ItemEquipable.hpp
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
#include "Enchantment.hpp"
#include "Controls.hpp"

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
            case slotNeck:
                return 'N';

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
            case slotNeck:
                return "Neck";

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

            case slotNeck:
                return "around";

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
            case slotNeck:
                return "Neck";

            case slotNone:
                return "None";
            default:
                return "ERR: "+to_string(slot);
        }
    }

    ItemEquipable() : Item() {

    }

    ItemEquipable(string name);

    virtual int getItemTypeId() {
        return itemTypeEquipable;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual bool equalsExceptQty(Item* other);

    virtual bool canBeEquipedHere(EquipSlot e) {
        if(Item::canBeEquipedHere(e)){
            return true;
        }
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

    ItemEquipable* addEnchantment(Enchantment e) {
        enchantments.push_back(e);
        return this;
    }

    vector<Enchantment> enchantments;

    bool operator< (ItemEquipable& b){
        if(this->getViableSlots().size() > 0 && b.getViableSlots().size() > 0){
            return this->getViableSlots()[0] < b.getViableSlots()[0];
        }else{
            return this->name < b.name;
        }
    }

    //int durability = 100;


protected:
};

#endif /* defined(__Underneath__ItemEquipable__) */

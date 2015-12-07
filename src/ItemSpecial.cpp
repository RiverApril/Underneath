//
//  ItemSpecial.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/2/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "ItemSpecial.hpp"
#include "Utility.hpp"

ItemSpecial::ItemSpecial(SpecialType specialty) : ItemSpecial(){
    this->specialty = specialty;

    switch (specialty) {

        case specialtyCoin:
            name = "Coin";
            artIndex = Arts::artCoin;
            break;

        case specialtyKey:
            name = "Key";
            artIndex = Arts::artSmallKey;
            break;

        case specialtyCompass:
            name = "Compass";
            break;

        case specialtyRepairer:
            name = "Repair Hammer";
            break;

        default:
            name = "UNDEFINED SPECIAL";
            break;
    }
}

void ItemSpecial::save(vector<unsigned char>* data){
    Item::save(data);
    Utility::saveInt(data, specialty);
}

void ItemSpecial::load(vector<unsigned char>* data, int* position){
    Item::load(data, position);
    specialty = Utility::loadInt(data, position);

}

ItemSpecial* ItemSpecial::cloneUnsafe(ItemSpecial* oldE, ItemSpecial* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->specialty = oldE->specialty;

    return newE;

}

bool ItemSpecial::equalsExceptQty(Item* other){
    ItemSpecial* otherW = dynamic_cast<ItemSpecial*>(other);
    return Item::equalsExceptQty(other)
    &&(otherW)
    &&(otherW->specialty == specialty);
}

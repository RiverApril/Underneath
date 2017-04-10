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
    specialty = (SpecialType)Utility::loadInt(data, position);

}

bool ItemSpecial::equalsExceptQty(Item* other){
    ItemSpecial* otherW = dynamic_cast<ItemSpecial*>(other);
    return (otherW)
    	&& Item::equalsExceptQty(other)
    	&& (otherW->specialty == specialty);
}

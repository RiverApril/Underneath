//
//  ItemPotion.cpp
//  Underneath
//
//  Created by Emily Atlee on 3/2/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "ItemPotion.hpp"
#include "Utility.hpp"

void ItemPotion::save(vector<unsigned char>* data) {
    Item::save(data);

    Utility::saveInt(data, (int) effects.size());
    for (Effect e : effects) {
        e.save(data);
    }
}

void ItemPotion::load(vector<unsigned char>* data, int* position) {
    Item::load(data, position);

    int size = Utility::loadInt(data, position);
    effects.clear();

    repeat(size, i) {
        effects.push_back(Effect(data, position));
    }
}

bool ItemPotion::operator< (ItemPotion& b){
    if(this->effects.size() > 0 && b.effects.size() > 0){
        if(this->effects[0].eId != b.effects[0].eId){
            return this->effects[0].eId < b.effects[0].eId;
        }else if(this->effects[0].meta != b.effects[0].meta){
            return this->effects[0].meta < b.effects[0].meta;
        }
    }
    return this->name < b.name;
}
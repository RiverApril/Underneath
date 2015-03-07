//
//  Enchantment.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/6/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Enchantment.h"
#include "Utility.h"

Enchantment::Enchantment(unsigned char* data, int* position){
    load(data, position);
}

Enchantment::Enchantment(EffectId effectId, int chance, double power, double time){
    this->effectId = effectId;
    this->chance = chance;
    this->power = power;
    this->time = time;
}

Enchantment Enchantment::setMeta(double meta){
    this->meta = meta;
    return *this;
}

void Enchantment::save(vector<unsigned char>* data){
    FileUtility::saveInt(data, effectId);
    FileUtility::saveInt(data, chance);
    FileUtility::saveDouble(data, power);
    FileUtility::saveDouble(data, time);
    FileUtility::saveDouble(data, meta);
}

void Enchantment::load(unsigned char* data, int* position){
    effectId = FileUtility::loadInt(data, position);
    chance = FileUtility::loadInt(data, position);
    power = FileUtility::loadDouble(data, position);
    time = FileUtility::loadDouble(data, position);
    meta = FileUtility::loadDouble(data, position);
}

bool operator== (const Enchantment a, const Enchantment b){
    return a.effectId == b.effectId
    && a.chance == b.chance
    && a.power == b.power
    && a.time == b.time;
}

bool operator!= (const Enchantment a, const Enchantment b){
    return !(a == b);
}

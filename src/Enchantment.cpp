//
//  Enchantment.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/6/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Enchantment.hpp"
#include "Utility.hpp"

Enchantment::Enchantment(unsigned char* data, int* position) {
    load(data, position);
}

Enchantment::Enchantment(EffectId effectId, int chance, double power, double time, double meta) {
    this->effectId = effectId;
    this->chance = chance;
    this->power = power;
    this->time = time;
    this->meta = meta;
}

void Enchantment::save(vector<unsigned char>* data) {
    Utility::saveInt(data, effectId);
    Utility::saveInt(data, chance);
    Utility::saveDouble(data, power);
    Utility::saveDouble(data, time);
    Utility::saveDouble(data, meta);
}

void Enchantment::load(unsigned char* data, int* position) {
    effectId = Utility::loadInt(data, position);
    chance = Utility::loadInt(data, position);
    power = Utility::loadDouble(data, position);
    time = Utility::loadDouble(data, position);
    meta = Utility::loadDouble(data, position);
}

bool operator==(const Enchantment a, const Enchantment b) {
    return a.effectId == b.effectId
            && a.chance == b.chance
            && a.power == b.power
            && a.time == b.time;
}

bool operator!=(const Enchantment a, const Enchantment b) {
    return !(a == b);
}

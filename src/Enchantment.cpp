//
//  Enchantment.cpp
//  Underneath
//
//  Created by Emily Atlee on 3/6/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "Enchantment.hpp"
#include "Utility.hpp"

Enchantment::Enchantment(vector<unsigned char>* data, int* position) {
    load(data, position);
}

Enchantment::Enchantment(Effect effect, EnchStyle style, int chance) {
    this->style = style;
    this->effect = effect;
    this->chance = chance;
}

void Enchantment::save(vector<unsigned char>* data) {
    effect.save(data);
    Utility::saveUnsignedChar(data, style);
    Utility::saveInt(data, chance);
}

void Enchantment::load(vector<unsigned char>* data, int* position) {
    effect.load(data, position);
    style = (EnchStyle)Utility::loadUnsignedChar(data, position);
    chance = Utility::loadInt(data, position);
}

bool operator==(const Enchantment a, const Enchantment b) {
    return a.style == b.style
    		&& a.chance == b.chance
    		&& a.effect == b.effect;
}

bool operator!=(const Enchantment a, const Enchantment b) {
    return !(a == b);
}

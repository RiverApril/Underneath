//
//  Enchantment.h
//  Underneath
//
//  Created by Braeden Atlee on 3/6/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Enchantment__
#define __Underneath__Enchantment__

#include "Effect.hpp"

struct Enchantment {
    Enchantment(unsigned char* data, int* position);

    Enchantment(EffectId effectId, int chance, double power, double time);

    Enchantment setMeta(double meta);

    void save(vector<unsigned char>* data);

    void load(unsigned char* data, int* position);

    EffectId effectId = effDamage;
    int chance = 1; //higher mean less likely
    double power = 1;
    double time = 0;
    double meta = 0;
    //if effectId is effDamage: meta is DamageType
};

static string enchantmentName(Enchantment e) {
    return effectName(e.effectId, e.meta);
}

bool operator==(const Enchantment a, const Enchantment b);

bool operator!=(const Enchantment a, const Enchantment b);

#endif /* defined(__Underneath__Enchantment__) */
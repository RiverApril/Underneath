//
//  Effect.h
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Effect__
#define __Underneath__Effect__

#include "DamageType.hpp"

#define EFFECT_DELAY 1

typedef int EffectId;

const EffectId effDamage = 0;
const EffectId effHeal = 1;
const EffectId effBuffAttack = 2;
const EffectId effBuffDefense = 3;
const EffectId effLSD = 4;
const EffectId effMemory = 5;

struct Effect {
    Effect(unsigned char* data, int* position);

    Effect(EffectId eId, double timeLeft, double power, double meta = 0);

    string toString();

    void save(vector<unsigned char>* data);

    void load(unsigned char* data, int* position);

    EffectId eId = effDamage;
    double timeLeft = 0;
    double power = 1;
    double meta = 0;
};

static string effectName(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeName((DamageType) meta);

        case effHeal:
            return "Heal";

        case effBuffAttack:
            return "Buff "+damageTypeName((DamageType) meta);

        case effBuffDefense:
            return "Resist "+damageTypeName((DamageType) meta);

        case effLSD:
            return "Psychedelic";

        case effMemory:
            return "Where am I?";

        default:
            return "UNDEFINED";
    }
}

static Ui::Color effectColor(EffectId eid, double meta) {
    switch (eid) {
        case effDamage:
            return damageTypeColor((DamageType) meta);

        case effHeal:
            return Ui::C_LIGHT_GREEN;

        case effBuffAttack:
            return Ui::C_LIGHT_BLUE;

        case effBuffDefense:
            return Ui::C_LIGHT_MAGENTA;

        default:
            return Ui::C_LIGHT_GREEN;
    }
}

bool operator==(const Effect a, const Effect b);

bool operator!=(const Effect a, const Effect b);

#endif /* defined(__Underneath__Effect__) */

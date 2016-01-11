//
//  Effect.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Effect__
#define __Underneath__Effect__

#include "DamageType.hpp"
#include "Color.hpp"

#define EFFECT_DELAY 1

typedef int EffectId;

const EffectId effDamage = 0;
const EffectId effHeal = 1;
const EffectId effBuffAttack = 2;
const EffectId effBuffDefense = 3;
const EffectId effLSD = 4;
const EffectId effMemory = 5;
const EffectId effPurity = 6;
const EffectId effBuffAbility = 7;

struct Effect {
    Effect(vector<unsigned char>* data, int* position);

    Effect(EffectId eId, double timeLeft, double power, double meta = 0);

    string toString();

    void save(vector<unsigned char>* data);

    void load(vector<unsigned char>* data, int* position);

    EffectId eId = effDamage;
    double timeLeft = 0;
    double power = 1;
    double meta = 0;
};

bool isBadEffect(Effect e);

string effectName(EffectId eid, double meta);

string effectPowerString(EffectId eid, double power);

Ui::Color effectColor(EffectId eid, double meta);

bool operator==(const Effect a, const Effect b);

bool operator!=(const Effect a, const Effect b);

#endif /* defined(__Underneath__Effect__) */

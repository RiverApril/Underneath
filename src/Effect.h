//
//  Effect.h
//  Underneath
//
//  Created by Braeden Atlee on 3/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Effect__
#define __Underneath__Effect__

#include "Global.h"

#define EFFECT_DELAY 1

typedef int EffectId;

const EffectId effDamage = 0;
const EffectId effHeal = 1;
const EffectId effBuff = 2;

struct Effect {
    Effect(unsigned char* data, int* position);

    Effect(EffectId eId, double timeLeft, double power);

    Effect setMeta(double meta);

    string toString();

    void save(vector<unsigned char>* data);

    void load(unsigned char* data, int* position);

    EffectId eId = effDamage;
    double timeLeft = 0;
    double power = 1;
    double meta = 0;
};

bool operator==(const Effect a, const Effect b);

bool operator!=(const Effect a, const Effect b);

#endif /* defined(__Underneath__Effect__) */

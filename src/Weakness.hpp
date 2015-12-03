//
//  Weakness.h
//  Underneath
//
//  Created by Braeden Atlee on 3/16/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Weakness__
#define __Underneath__Weakness__

#include "ItemWeapon.hpp"

struct Weakness {

    Weakness(DamageType dam, double m) {
        this->damageType = dam;
        this->multiplier = m;
    }
    DamageType damageType = damSharp;
    double multiplier = 1;

    void save(vector<unsigned char>* data);

    void load(vector<unsigned char>* data, int* position);
};

#endif /* defined(__Underneath__Weakness__) */

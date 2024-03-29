//
//  Weakness.hpp
//  Underneath
//
//  Created by Emily Atlee on 3/16/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#ifndef __Underneath__Weakness__
#define __Underneath__Weakness__

#include "ItemWeapon.hpp"

struct Weakness {

    Weakness(DamageType dam, double m) : damageType(dam), multiplier(m){}
    
    Weakness(vector<unsigned char>* data, int* position) {
        load(data, position);
    }

    DamageType damageType = damSharp;
    double multiplier = 1;

    void save(vector<unsigned char>* data);

    void load(vector<unsigned char>* data, int* position);
};

#endif /* defined(__Underneath__Weakness__) */

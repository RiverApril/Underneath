//
//  Enchantment.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/6/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Enchantment__
#define __Underneath__Enchantment__

#include "Effect.hpp"


enum EnchStyle{
eStyle_SelfToEnemy_EnemyEff,    //Applied to enemy when weapon hits enemy s  -> e*
eStyle_SelfToEnemy_SelfEff,     //Applied to self when weapon hits enemy  s* -> e
eStyle_onTick_SelfEff,          //Applied to self on tick                 s*
eStyle_EnemyToSelf_SelfEff,     //Applied to self when hit by enemy       s* <- e
eStyle_EnemyToSelf_EnemyEff,    //Applied to enemy when hit by enemy      s  <- e*
};

struct Enchantment {
    Enchantment(vector<unsigned char>* data, int* position);

    Enchantment(Effect effect, EnchStyle style, int chance);

    void save(vector<unsigned char>* data);

    void load(vector<unsigned char>* data, int* position);

    Effect effect = Effect();
    int chance = 1; //higher mean less likely (1 out of chance)
    EnchStyle style = eStyle_SelfToEnemy_EnemyEff;
};

bool operator==(const Enchantment a, const Enchantment b);

bool operator!=(const Enchantment a, const Enchantment b);

#endif /* defined(__Underneath__Enchantment__) */

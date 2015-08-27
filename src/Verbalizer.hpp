//
//  Verbalizer.h
//  Underneath
//
//  Created by Braeden Atlee on 2/27/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Verbalizer__
#define __Underneath__Verbalizer__

#include "Global.hpp"
#include "EntityPlayer.hpp"

namespace Verbalizer {

    void weakness(EntityAlive* underAttack, Weakness w, double damageWithoutChange, double damageWithChange);

    void attack(EntityAlive* attacker, EntityAlive* underAttack, ItemWeapon* weapon, double damage);

    void defeatedEnemy(EntityAlive* dead, int xpGained);

    void leveledUp(EntityPlayer* player);

}

#endif /* defined(__Underneath__Verbalizer__) */

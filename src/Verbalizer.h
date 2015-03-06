//
//  Verbalizer.h
//  Underneath
//
//  Created by Braeden Atlee on 2/27/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Verbalizer__
#define __Underneath__Verbalizer__

#include "Global.h"
#include "Player.h"

namespace Verbalizer {

    void attack(Alive* attacker, Alive* underAttack, Weapon* weapon, double damage);

    // Alive* dead  lol
    void defeatedEnemy(Alive* dead, int xpGained);

    void leveledUp(Player* player);

}

#endif /* defined(__Underneath__Verbalizer__) */

//
//  CombatSpell.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/16/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "CombatSpell.h"
#include "Utility.h"

void CombatSpell::save(vector<unsigned char>* data) {
    Ranged::save(data);

    Utility::saveInt(data, manaCost);
}

void CombatSpell::load(unsigned char* data, int* position) {
    Ranged::load(data, position);

    manaCost = Utility::loadInt(data, position);
}

CombatSpell* CombatSpell::cloneUnsafe(CombatSpell* oldE, CombatSpell* newE) {

    Ranged::cloneUnsafe(oldE, newE);

    newE->manaCost = oldE->manaCost;


    return newE;

}

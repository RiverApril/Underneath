//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "Spell.h"


namespace ItemGenerator {

    Weapon* setDamageType(Weapon* w, DamageType d);

    struct CombatLevel {
        double multiplier;
        string name;
        CombatLevel(double m, string n){
            this->multiplier = m;
            this->name = n;
        }
    };
    extern CombatLevel combat0Abysmal;
    extern CombatLevel combat1Shitty;
    extern CombatLevel combat2Training;
    extern CombatLevel combat3Basic;
    extern CombatLevel combat4Apprentice;
    extern CombatLevel combat5Skillful;
    extern CombatLevel combat6Good;


    extern Weapon* wKnife;
    extern Weapon* wShortSword;
    extern Weapon* wLongSword;
    extern Weapon* wBattleAxe;
    extern Weapon* wMase;
    extern Weapon* wSpear;

    extern Ranged* wRecurveBow;
    extern Ranged* wLongbow;
    extern Ranged* wCrossbow;


    extern vector<CombatLevel> combatLevelList;
    extern vector<Weapon*> weaponList;

    Weapon* atl(Weapon* w);
    CombatLevel atl(CombatLevel c);


    Weapon* createWeaponBase(DamageType d);
    Weapon* createWeapon(string name, CombatLevel combatLevel, DamageType damageType, bool enchanted);


}


#endif /* defined(__Underneath__ItemGenerator__) */

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

    extern Item* iCoin;

    extern Weapon* wKnife;
    extern Weapon* wShortSword;
    extern Weapon* wLongSword;
    extern Weapon* wBattleAxe;
    extern Weapon* wMase;
    extern Weapon* wSpear;

    extern Ranged* wRecurveBow;
    extern Ranged* wLongbow;
    extern Ranged* wCrossbow;

    extern Spell* wCombatSpell;

	
    extern vector<Weapon*> weaponList;

    Weapon* atl(Weapon* w);


    vector<Item*> createLoots(int difficulty);
    Weapon* createWeaponBase(DamageType d);

    Weapon* createWeapon(int itemDifficulty);
    Weapon* createWeapon(int itemDifficulty, DamageType damageType);
    Weapon* createWeapon(Material* material, DamageType damageType, bool enchanted);
    

    void initItemTemplates();
    void cleanupItemTemplates();


}


#endif /* defined(__Underneath__ItemGenerator__) */
